/*******************************************************************************
 * 文件名称: hooks
 * 项目名称: EFModLoader
 * 创建时间: 2024/12/7
 * 作者: EternalFuture゙
 * Github: https://github.com/2079541547 
 * 版权声明: Copyright © 2024 EternalFuture. All rights reserved.
 * 许可证: This program is free software: you can redistribute it and/or modify
 *         it under the terms of the GNU Affero General Public License as published
 *         by the Free Software Foundation, either version 3 of the License, or
 *         (at your option) any later version.
 *
 *         This program is distributed in the hope that it will be useful,
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *         GNU Affero General Public License for more details.
 *
 *         You should have received a copy of the GNU Affero General Public License
 *         along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * 描述信息: 本文件为EFModLoader项目中的一部分。
 * 注意事项: 请严格遵守GNU AGPL v3.0协议使用本代码，任何未经授权的商业用途均属侵权行为。
 *******************************************************************************/

#include <hook/hooks.hpp>
#include <SilkHash64.hpp>


std::vector<void*> EFModLoader::Hooks::getHooksFunc(EFModLoader::Hooks::HOOKS hook) {
    std::vector<void*> result;
    for (auto HOOK: hookVector_Mod) {
        for (auto PTR: HOOK[SilkHash64::hashString(hook.getKey())]) {
            result.push_back(PTR);
        }
    }
    return result;
}

void EFModLoader::Hooks::registerHook(const EFModLoader::Hooks::HOOKS& hook,
                                      void *ptr,
                                      void *new_ptr,
                                      void **old_ptr) {

    hookMap[SilkHash64::hashString(hook.getKey())] = {ptr, new_ptr, old_ptr};
}