# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(dronetag_task)

zephyr_include_directories(src)

target_sources(app PRIVATE 
            src/main.c
            src/my_led.c
            src/my_btn.c
            src/my_msgq.c
            src/my_fifo.c
            src/my_linked_list.c)

