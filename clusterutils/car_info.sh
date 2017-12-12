#!/bin/bash

mpic++ -std=gnu++11 /home/nikolay.galko/main_info_only.cpp -o /home/nikolay.galko/main_info_only

mpirun /home/nikolay.galko/main_info_only