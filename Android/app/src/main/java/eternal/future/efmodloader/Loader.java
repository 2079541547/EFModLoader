package eternal.future.efmodloader;

import android.annotation.SuppressLint;
import android.os.Build;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

/*******************************************************************************
 * 文件名称: Loader
 * 项目名称: EFModLoader
 * 创建时间: 2024/11/2 13:42
 * 作者: EternalFuture゙
 * Github: https://github.com/2079541547
 * 版权声明: Copyright © 2024 EternalFuture゙. All rights reserved.
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
 * 描述信息: 本文件为EFModLoader项目中的一部分。
 * 注意事项: 请严格遵守GNU AGPL v3.0协议使用本代码，任何未经授权的商业用途均属侵权行为。
 *******************************************************************************/

public class Loader {

    private static final Map<String, String> archToLib = new HashMap<String, String>(4);

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public static void load(){
        System.load(Objects.requireNonNull(Loader.class.getClassLoader()).getResource("assets/EFModLoader/" + Build.CPU_ABI + "/").getPath());
    }


}
