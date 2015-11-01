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

#ifndef CATCHERS_H
#define CATCHERS_H

#include <exception>

#define CATCH_TYPES_EWHAT0(body) \
    catch(...) {                            \
        const char * EWHAT = "unknown exception";    \
        body   \
    }

#define CATCH_TYPES_EWHAT1(t1, body) \
    catch(t1 & e) {              \
        const char * EWHAT = e.what();    \
        body                      \
    } \
    CATCH_TYPES_EWHAT0(body)

#define CATCH_TYPES_EWHAT2(t1, t2, body) \
    catch(t1 & e) {              \
        const char * EWHAT = e.what();    \
        body                      \
    }                                       \
    CATCH_TYPES_EWHAT1(t2, body)

#define CATCH_STD_ALL_EWHAT(body)         \
        CATCH_TYPES_EWHAT1(std::exception, body)

#define CATCH_STD_LOGLEVEL_IGNORE(msg, loglevel) \
   catch (std::exception& e) {      \
       LOG_##loglevel (msg << ": " << e.what() << " - ignored"); \
   }

#define CATCH_STD_ALL_LOGLEVEL_IGNORE(msg, loglevel) \
        CATCH_STD_ALL_EWHAT( \
            LOG_##loglevel(msg << ": " << EWHAT << " - ignored"); )

#define CATCH_STD_ALL_LOG_IGNORE(msg) \
    CATCH_STD_ALL_LOGLEVEL_IGNORE(msg, ERROR)

#define CATCH_STD_ALL_LOGLEVEL_RETHROW(msg, loglevel) \
        CATCH_STD_ALL_EWHAT( \
            LOG_##loglevel(msg << ": " << EWHAT); \
            throw; )

#define CATCH_STD_ALL_LOG_RETHROW(msg) \
        CATCH_STD_ALL_LOGLEVEL_RETHROW(msg, ERROR)

#define CATCH_STD_ALL_LOG_THROWFUNGI(msg) \
        CATCH_STD_ALL_EWHAT( \
            LOG_ERROR(msg << ": " << EWHAT); \
            throw fungi::IOException(msg); )

#define CATCH_STD_ALL_LOGLEVEL_ADDERROR_RETHROW(msg, loglevel, errortype) \
    CATCH_STD_ALL_EWHAT({                                               \
            std::stringstream ss;                                       \
            ss << msg << ": " << EWHAT;                                 \
            LOG_##loglevel(ss.str().c_str());                           \
                VolumeDriverError::report(errortype,                    \
                                          ss.str());                    \
                throw;                                                  \
        })

#define CATCH_STD_ALL_LOG_ADDERROR_RETHROW(msg, errortype) \
    CATCH_STD_ALL_LOGLEVEL_ADDERROR_RETHROW(msg, ERROR, errortype)


//using the following requires a
//#define GETVOLUME()

#define LOG_VTRACE(msg)                         \
    LOG_TRACE(GETVOLUME()->getName() << ": " << msg)

#define LOG_VDEBUG(msg)                         \
    LOG_DEBUG(GETVOLUME()->getName() << ": " << msg)

#define LOG_VPERIODIC(msg)                          \
    LOG_PERIODIC(GETVOLUME()->getName() << ": " << msg)

#define LOG_VINFO(msg)                          \
    LOG_INFO(GETVOLUME()->getName() << ": " << msg)

#define LOG_VWARN(msg)                          \
    LOG_WARN(GETVOLUME()->getName() << ": " << msg)


#define LOG_VERROR(msg)                         \
    LOG_ERROR(GETVOLUME()->getName() << ": " << msg)

#define LOG_VFATAL(msg)                         \
    LOG_FATAL(GETVOLUME()->getName() << ": " << msg)

#define CATCH_STD_VLOG_IGNORE(msg, loglevel) \
   catch (std::exception& e) {      \
       LOG_V##loglevel (msg << ": " << e.what() << " - ignored"); \
   }

#define CATCH_STD_ALL_VLOGLEVEL_IGNORE(msg, loglevel) \
        CATCH_STD_ALL_EWHAT( \
            LOG_V##loglevel(msg << ": " << EWHAT << " - ignored"); )

#define CATCH_STD_ALL_VLOG_IGNORE(msg) \
    CATCH_STD_ALL_VLOGLEVEL_IGNORE(msg, ERROR)

#define CATCH_STD_ALL_VLOG_RETHROW(msg) \
        CATCH_STD_ALL_EWHAT( \
            LOG_VERROR(msg << ": " << EWHAT); \
            throw; )

#define CATCH_STD_ALL_VLOG_HALT_RETHROW(msg) \
        CATCH_STD_ALL_EWHAT( \
            LOG_VERROR(msg << ": " << EWHAT); \
            GETVOLUME()->halt(); \
            throw; )

#define CATCH_STD_ALL_VLOGLEVEL_ADDERROR_RETHROW(msg, loglevel, errortype) \
    CATCH_STD_ALL_EWHAT({                                               \
            std::stringstream ss;                                       \
            ss << msg << ": " << EWHAT;                                 \
            LOG_V##loglevel(ss.str().c_str());                          \
                VolumeDriverError::report(errortype,                    \
                                          ss.str().c_str(),             \
                                          GETVOLUME()->getName());      \
                throw;                                                  \
        })

#define CATCH_STD_ALL_VLOG_ADDERROR_RETHROW(msg, errortype) \
    CATCH_STD_ALL_VLOGLEVEL_ADDERROR_RETHROW(msg, ERROR, errortype)

#define CATCH_STD_ALL_VLOG_ADDERROR_HALT_RETHROW(msg, errortype) \
    CATCH_STD_ALL_EWHAT({                                        \
            std::stringstream ss;                                \
            ss << msg << ": " << EWHAT;                          \
            LOG_VERROR(ss.str().c_str());                        \
            VolumeDriverError::report(errortype,                 \
                                      ss.str().c_str(),          \
                                      GETVOLUME()->getName());   \
                                                                 \
            GETVOLUME()->halt();                                 \
            throw;                                               \
        })

#define CATCH_STD_ALL_VLOG_ADDERROR_HALT(msg, errortype)        \
    CATCH_STD_ALL_EWHAT({                                       \
            std::stringstream ss;                               \
            ss << msg << ": " << EWHAT;                         \
            LOG_VERROR(ss.str().c_str());                       \
            VolumeDriverError::report(errortype,                \
                                      ss.str().c_str(),         \
                                      GETVOLUME()->getName());  \
            GETVOLUME()->halt();                                \
        })

#define CATCH_STD_ALL_VLOG_THROWFUNGI(msg) \
        CATCH_STD_ALL_EWHAT( \
            LOG_VERROR(msg << ": " << EWHAT); \
            throw fungi::IOException(msg); )


#endif
