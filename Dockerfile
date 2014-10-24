FROM ubuntu:14.04
MAINTAINER yanyiwu <wuyanyi09@foxmail.com>
RUN apt-get update
RUN apt-get install -y g++ cmake git
RUN git clone https://github.com/aszxqw/cppjieba.git
RUN mkdir cppjieba/build
WORKDIR /cppjieba/build
RUN cmake .. 
RUN make
EXPOSE 11200
CMD ["./bin/cjserver", "../test/testdata/server.conf"]
