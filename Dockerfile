FROM ubuntu:latest

RUN apt update
RUN apt install -y cmake build-essential libpqxx-dev

WORKDIR /minor_server

COPY ./source ./source
COPY ./include ./include

RUN mkdir -p cmake/build
RUN cd cmake/build && cmake ../.. && make

CMD ./cmake/build/minor_server