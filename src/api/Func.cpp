/*******************************************************************************
 * 文件名称: Func
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

#include <api/Func.hpp>
#include <SilkHash64.hpp>
#include <Log.hpp>

void EFModLoader::API::Func::registerFunc(EFModLoader::API::Func::EFModAPI_Func FUNC, void *PTR) {
    // 计算函数的唯一标识符（ID），通过哈希函数键值来生成。
    uint64_t funcId = SilkHash64::hashString(FUNC.getKey());

    // 将函数ID和指向函数的指针添加到全局函数映射表中。
    FuncMap[funcId] = PTR;

    EFLOG(LogLevel::INFO, "EFModLoader::API::Func", "registerFunc",
          "成功注册函数：" + FUNC.getKey() + " ID: " + std::to_string(funcId));
}