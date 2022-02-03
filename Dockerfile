FROM ubuntu-upstart:14.04

ENV SUBSCRIBER_REPO_DIR=~/subscriptions

RUN apt-get update && apt-get install --assume-yes build-essential awscli git jq curl expect-dev

COPY . /bottle-src
RUN cd /bottle-src && make install
