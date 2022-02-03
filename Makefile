.PHONY: build install start push run image-name clean shell
SHELL:=/bin/bash
IMAGE_NAME:=$(shell basename $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST)))))
ENV_FILE_PARAM=$(shell test -n "$(ENVIRONMENT_FILE)" && echo --env-file $(ENVIRONMENT_FILE))

debug:
	@echo $(ENV_FILE_PARAM)

image-name:
	@echo $(IMAGE_NAME)

clean:
	-@docker image rm -f $(IMAGE_NAME)
	-@docker rm $(IMAGE_NAME)

build:
	docker build . -t $(IMAGE_NAME):latest

run: build
	docker run --name $(IMAGE_NAME) $(ENV_FILE_PARAM) --rm -i $(IMAGE_NAME):latest

shell: build
	docker run --name $(IMAGE_NAME) $(ENV_FILE_PARAM) --rm --entrypoint /bin/bash -it $(IMAGE_NAME):latest 

attach-shell:
	docker exec -it $(IMAGE_NAME) /bin/bash

install:
	# yup, starphleet is gonna run us as ubuntu
	sudo ./install ubuntu
	# the upstart scripts expect the app to live at ~/bottle
	ln -s ${PWD} ~/bottle

start:
	# we don't have our 'start' file created until, well, now.
	# and we can get anything we might need from our orders which should
	# be the 3rd space delimited field of the 'container_environment' line from
	# our /home/ubuntu/start file, yup super dependent on starphleet structure
	# 
	# just to keep things clean, we'll filter out the autodeploy directive so we 
	# don't get extraneous errors
	sudo start bottle || true
	# now for a couple extra workers, as this is what we run traditionally
	sudo start bottle-router name=router1 || true
	sudo start bottle-router name=router2 || true
	sudo start bottle-task-worker name=task_worker1 || true
	sudo start bottle-task-worker name=task_worker2 || true
	sudo start bottle-task-worker name=task_worker3 || true
	sudo start bottle-task-worker name=task_worker4 || true
