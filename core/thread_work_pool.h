/*************************************************************************/
/*  thread_work_pool.h                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef THREAD_WORK_POOL_H
#define THREAD_WORK_POOL_H

#include "core/os/memory.h"
#include "core/os/semaphore.h"
#include <atomic>
#include <thread>
class ThreadWorkPool {

	std::atomic<uint32_t> index;

	struct BaseWork {
		std::atomic<uint32_t> *index;
		uint32_t max_elements;
		virtual void work() = 0;
		virtual ~BaseWork() = default;
	};

	template <class C, class M, class U>
	struct Work : public BaseWork {
		C *instance;
		M method;
		U userdata;
		virtual void work() {

			while (true) {
				uint32_t work_index = index->fetch_add(1, std::memory_order_relaxed);
				if (work_index >= max_elements) {
					break;
				}
				(instance->*method)(work_index, userdata);
			}
		}
	};

	struct ThreadData {
		std::thread *thread;
		Semaphore start;
		Semaphore completed;
		std::atomic<bool> exit;
		BaseWork *work;
	};

	ThreadData *threads = nullptr;
	uint32_t thread_count = 0;

	static void _thread_function(ThreadData *p_thread);

public:
	template <class C, class M, class U>
	void do_work(uint32_t p_elements, C *p_instance, M p_method, U p_userdata) {

		ERR_FAIL_COND(!threads); //never initialized

		index.store(0);

		Work<C, M, U> *w = memnew((Work<C, M, U>));
		w->instance = p_instance;
		w->userdata = p_userdata;
		w->method = p_method;
		w->index = &index;
		w->max_elements = p_elements;

		for (uint32_t i = 0; i < thread_count; i++) {
			threads[i].work = w;
			threads[i].start.post();
		}
		for (uint32_t i = 0; i < thread_count; i++) {
			threads[i].completed.wait();
			threads[i].work = nullptr;
		}

		memdelete(w);
	}

	void init(int p_thread_count = -1);
	void finish();
	~ThreadWorkPool();
};

#endif // THREAD_POOL_H
