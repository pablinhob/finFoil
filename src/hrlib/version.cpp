/****************************************************************************
  
 Copyright (c) 2013, Hans Robeers
 All rights reserved.
 
 BSD 2-Clause License
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
   * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   
   * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
   
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
****************************************************************************/

#include "version.h"

using namespace hrlib;

Version::Version(int major, int minor, int revision, int build, QString commit, ReleaseType::e releaseType)
{
    _major = major;
    _minor = minor;
    _revision = revision;
    _build = build;
    _commit = commit;
    _releaseType = releaseType;

    // Create string
    _string.append(QString::number(_major)).append('.').append(QString::number(_minor)).append('.')
            .append(QString::number(_revision)).append('.').append(QString::number(_build));

    switch (_releaseType)
    {
    case ReleaseType::PreAlpha:
        _string.append(" pre-alpha");
        break;

    case ReleaseType::Aplha:
        _string.append(" alpha");
        break;

    case ReleaseType::Beta:
        _string.append(" beta");
        break;

    case ReleaseType::Nightly:
        _string.append(" nightly");
        break;

    case ReleaseType::Release:
        break;
    }
}

QString hrlib::Version::toString()
{
    return _string;
}

QString Version::commit()
{
    return _commit;
}
