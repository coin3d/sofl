/**************************************************************************\
 * BSD 3-Clause License
 *
 * Copyright (c) 2025, Fabrizio Morciano
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#define BOOST_TEST_NO_LIB 1
#include <boost/test/unit_test.hpp>
#include "Inventor/Fl/SoFl.h"
#include <FL/Enumerations.H>

BOOST_AUTO_TEST_SUITE(TestSoFl);

std::string 
buildVersion(int major, 
             int minor,
             int release) {
    std::ostringstream oss;
    oss<<major<<'.'<<minor<<'.'<<release;
    return (oss.str());
}

BOOST_AUTO_TEST_CASE(shouldVerifyVersion) {
    int major,minor,release;
    SoFl::getVersionInfo(&major,&minor,&release);
    BOOST_CHECK_EQUAL(major, 0);
    BOOST_CHECK_EQUAL(minor, 0);
    BOOST_CHECK_EQUAL(release, 4);

    std::string oracle = buildVersion(major,minor,release);
    std::string s1 = SoFl::getVersionString();
    BOOST_CHECK_EQUAL(s1, oracle);

    oracle = buildVersion(FL_MAJOR_VERSION, FL_MINOR_VERSION,FL_PATCH_VERSION);
    s1 = SoFl::getVersionToolkitString();
    BOOST_CHECK_EQUAL(s1, oracle);
}


BOOST_AUTO_TEST_SUITE_END();
