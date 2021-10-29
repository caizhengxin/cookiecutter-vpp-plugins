/**
 * @Author: jankincai
 * @Date:   2021-09-22 10:57:23
 * @Last Modified by:   jankincai
 * @Last Modified time: 2021-10-26 11:09:46
 */

/*
 * {{cookiecutter.project_alias}}.h - skeleton vpp engine plug-in header file
 *
 * Copyright (c) <current-year> <your-organization>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
{% set prefix = cookiecutter.project_alias[0] %}#ifndef __included_{{cookiecutter.project_alias}}_h__
#define __included_{{cookiecutter.project_alias}}_h__

#include <vnet/vnet.h>
#include <vnet/ip/ip.h>
#include <vnet/ethernet/ethernet.h>

#include <vppinfra/hash.h>
#include <vppinfra/error.h>

typedef struct {
    /* API message ID base */
    u16 msg_id_base;

    /* on/off switch for the periodic function */
    u8 periodic_timer_enabled;
    /* Node index, non-zero if the periodic process has been created */
    u32 periodic_node_index;

    /* convenience */
    vlib_main_t *vlib_main;
    vnet_main_t *vnet_main;
    ethernet_main_t *ethernet_main;
}{{cookiecutter.project_alias}}_main_t;

extern {{cookiecutter.project_alias}}_main_t {{cookiecutter.project_alias}}_main;

extern vlib_node_registration_t {{cookiecutter.project_alias}}_node;
extern vlib_node_registration_t {{cookiecutter.project_alias}}_periodic_node;

/* Periodic function events */
#define {{cookiecutter.project_alias.upper()}}_EVENT1 1
#define {{cookiecutter.project_alias.upper()}}_EVENT2 2
#define {{cookiecutter.project_alias.upper()}}_EVENT_PERIODIC_ENABLE_DISABLE 3


void {{cookiecutter.project_alias}}_create_periodic_process({{cookiecutter.project_alias}}_main_t *);

extern int {{cookiecutter.project_alias}}_enable_disable({{cookiecutter.project_alias}}_main_t *{{prefix}}mp, int enable_disable);

#endif /* __included_{{cookiecutter.project_alias}}_h__ */

/*
 * fd.io coding-style-patch-verification: ON
 *
 * Local Variables:
 * eval: (c-set-style "gnu")
 * End:
 */

