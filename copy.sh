#! /bin/bash
rsync -ru -e 'ssh -p 2222 -o StrictHostKeyChecking=no' --progress --exclude '.git/' . iboutsikas@linux-dev.local:~/CMSC483-691Project