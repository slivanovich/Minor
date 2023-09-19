FROM ubuntu:latest

RUN apt update --fix-missing
RUN apt install -y cmake build-essential libpqxx-dev

WORKDIR /minor_server

COPY ./CMakeLists.txt ./CMakeLists.txt
COPY ./source ./source
COPY ./include ./include

RUN mkdir -p cmake/build
RUN cd cmake/build && cmake ../.. && make

CMD ./cmake/build/minor_server