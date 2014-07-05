/*
 * common.h
 *
 *  Created on: Jan 20, 2014
 *      Author: rodtoll
 */

#ifndef COMMON_H_
#define COMMON_H_

#define BOOST_LOG_TRIVIAL(x) cout

#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/container/map.hpp>
#include <boost/filesystem.hpp>

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <signal.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

using namespace std;
using namespace boost;

#endif /* COMMON_H_ */
