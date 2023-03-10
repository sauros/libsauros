#include "cell.hpp"
#include "libsauros/processor/processor.hpp"
#include <sstream>

#include <iostream>

namespace sauros {

async_cell_c::async_cell_c(location_s *location)
    : variant_cell_c(cell_variant_type_e::ASYNC, location) {

  get_fn = create_cell(
      [this](cells_t &cells, env_ptr env) -> cell_t { return future.get(); });

  wait_fn =
      create_cell([this](cells_t &cells, env_ptr env) -> cell_t {
        future.wait();
        return create_cell(CELL_TRUE);
      });
}

thread_cell_c::thread_cell_c(location_s *location)
    : variant_cell_c(cell_variant_type_e::THREAD, location) {

  is_joinable =
      create_cell([this](cells_t &cells, env_ptr env) -> cell_t {
        return create_cell(cell_type_e::INTEGER,
                                        std::to_string(thread.joinable()));
      });

  join =
      create_cell([this](cells_t &cells, env_ptr env) -> cell_t {
        try {
          thread.join();
        } catch (...) {
          throw exceptions::runtime_c("unable to join thread", cells[0]);
        }
        return create_cell(CELL_TRUE);
      });

  detach =
      create_cell([this](cells_t &cells, env_ptr env) -> cell_t {
        thread.detach();
        return create_cell(CELL_TRUE);
      });

  get_id =
      create_cell([this](cells_t &cells, env_ptr env) -> cell_t {
        std::stringstream ss;
        ss << thread.get_id();
        cell_int_t id{0};
        ss << id;
        return create_cell(cell_type_e::INTEGER, id);
      });
}

chan_cell_c::chan_cell_c(location_s *location)
    : variant_cell_c(cell_variant_type_e::CHAN, location) {

  put_fn =
      create_cell([this](cells_t &cells, env_ptr env) -> cell_t {
        if (cells.size() == 1) {
          return create_cell(CELL_FALSE);
        }
        const std::lock_guard<std::mutex> lock(channel_mutex);
        for (auto it = cells.begin() + 1; it != cells.end(); ++it) {
          channel_queue.push(processor->process_cell((*it), env));
        }
        return create_cell(CELL_TRUE);
      });

  has_data_fn = create_cell([this](cells_t &cells,
                                                env_ptr env) -> cell_t {
    const std::lock_guard<std::mutex> lock(channel_mutex);
    return create_cell(
        cell_type_e::INTEGER, static_cast<cell_int_t>(!channel_queue.empty()));
  });

  get_fn =
      create_cell([this](cells_t &cells, env_ptr env) -> cell_t {
        cell_t next;
        {
          const std::lock_guard<std::mutex> lock(channel_mutex);
          if (channel_queue.empty()) {
            return create_cell(CELL_NIL);
          }
          next = channel_queue.front();
          channel_queue.pop();
        }
        return next;
      });

  drain_fn =
      create_cell([this](cells_t &cells, env_ptr env) -> cell_t {
        const std::lock_guard<std::mutex> lock(channel_mutex);
        cell_t result = create_cell(cell_type_e::LIST);
        while (!channel_queue.empty()) {
          result->list.push_back(channel_queue.front());
          channel_queue.pop();
        }
        return result;
      });
}

ref_cell_c::ref_cell_c(location_s *location)
    : variant_cell_c(cell_variant_type_e::REF, location) {

  put_fn =
      create_cell([this](cells_t &cells, env_ptr env) -> cell_t {
        if (cells.size() != 2) {
          throw exceptions::runtime_c("ref_cell.put expects 1 parameters",
                                      cells[0]);
        }
        const std::lock_guard<std::mutex> lock(ref_mut);
        ref_value = cells[1];
        return create_cell(CELL_TRUE);
      });

  get_fn =
      create_cell([this](cells_t &cells, env_ptr env) -> cell_t {
        if (cells.size() != 1) {
          throw exceptions::runtime_c("ref_cell.get expects no parameters",
                                      cells[0]);
        }
        const std::lock_guard<std::mutex> lock(ref_mut);
        return ref_value;
      });
}

void_cell_c::void_cell_c(location_s *location)
    : variant_cell_c(cell_variant_type_e::VOID, location) {}

void_cell_c::~void_cell_c() {
  if (deletion_cb) {
    deletion_cb(ptr);
  }
}

} // namespace sauros