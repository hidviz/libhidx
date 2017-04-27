#!/bin/sh
if [ -d asio ];then
exit 0
fi
git clone --branch asio-1-10-8 --depth=1 https://github.com/chriskohlhoff/asio.git


