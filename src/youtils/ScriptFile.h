// Copyright 2015 iNuron NV
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _SCRIPT_FILE_H_
#define _SCRIPT_FILE_H_

#include <vector>
#include <string>
#include "FileUtils.h"
#include "Assert.h"
#include <boost/filesystem/fstream.hpp>
#include <sys/stat.h>

namespace youtils
{
struct ScriptFile
{
    DECLARE_LOGGER("ScriptFile");

    ScriptFile(const fs::path& p,
               const std::vector<std::string>& lines)
    {
        boost::filesystem::ofstream os(p);
        os << "#! /bin/bash" << std::endl;
        os << "set -eux" << std::endl;
        for(const std::string& line : lines)
        {
            os << line << std::endl;
        }
        int res = chmod(p.string().c_str(),0777);
        VERIFY(res == 0);
    }
};

}

#endif // _SCRIPT_FILE_H_
