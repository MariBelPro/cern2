#!/bin/bash

if [ $# -ne 2 ] ; then
    echo "Usage: sh GenerateList.sh <dataset> <number_of_files_per_task>"
    exit
fi

# The STAR data are stored here
export DATA_DIR=/mnt/pool/6/nigmatkulov/star/femtoDst/19p6gev
#export DATASET=$1

if [ ${DATASET} != "7gev" ]  && \
   [ ${DATASET} != "11gev" ] && \
   [ ${DATASET} != "14gev" ] && \
   [ ${DATASET} != "19gev" ] && \
   [ ${DATASET} != "27gev" ] && \
   [ ${DATASET} != "39gev" ] && \
   [ ${DATASET} != "62gev" ] ; then
  echo "Sorry! For the time being, only the following datasets are available:"
  echo "7gev  11gev  14gev  19gev  27gev  39gev  62gev"
  exit
fi 

export N_FILES_PER_TASK=$2

# The main working directory
# export MAIN_DIR=`dirname "$( readlink -f ${BASH_SOURCE}; )"`/runlist
export MAIN_DIR="$(dirname "$( readlink -f -- "$0" )")"/runlist
mkdir -p ${MAIN_DIR}

# The input file list 
export FILELIST=${MAIN_DIR}/runlist_slurm_${DATASET}_${N_FILES_PER_TASK}FilesPerTask.list

# Checking for the filelist existence in MAIN_DIR
if [ -f ${FILELIST} ] ; then
  echo "This list has already been created."
  exit
fi

echo "Initializing a list containing lists of ROOT files (Inception by Christopher Nolan)"
if [ ! -d ${MAIN_DIR}/split/${DATASET}_${N_FILES_PER_TASK}FilesPerTask ]; then
  mkdir -p ${MAIN_DIR}/split/${DATASET}_${N_FILES_PER_TASK}FilesPerTask
else
  echo "This list has already been created."
  export N_TASKS=$( ls ${MAIN_DIR}/split/${DATASET}_${N_FILES_PER_TASK}FilesPerTask | wc -l )
  echo "Number of needed job array for ${DATASET} dataset: ${N_TASKS}"
  exit
fi

echo "Listing all ROOT tree of $DATASET data set (Inception - level 1)"
if [ ! -f ${MAIN_DIR}/split/runlist_${DATASET}.list ] ; then
  find ${DATA_DIR}/data1/ -type f -name "*.femtoDst.root" > ${MAIN_DIR}/split/runlist_${DATASET}.list
  find ${DATA_DIR}/data0/ -type f -name "*.femtoDst.root" >> ${MAIN_DIR}/split/runlist_${DATASET}.list
  find ${DATA_DIR}/data2/ -type f -name "*.femtoDst.root" >> ${MAIN_DIR}/split/runlist_${DATASET}.list

  
fi

echo "Creating list for each job (Inception - level 2)"
split -l${N_FILES_PER_TASK} -d ${MAIN_DIR}/split/runlist_${DATASET}.list ${MAIN_DIR}/split/${DATASET}_${N_FILES_PER_TASK}FilesPerTask/runlist_${DATASET}_
for filename in ${MAIN_DIR}/split/${DATASET}_${N_FILES_PER_TASK}FilesPerTask/runlist_${DATASET}_*;do mv "$filename" "$filename.list"; done;


echo "Initializing runlist for SGE JOB (Inception - level 3)"
ls ${MAIN_DIR}/split/${DATASET}_${N_FILES_PER_TASK}FilesPerTask/runlist_${DATASET}_*.list > ${FILELIST}
echo "Non, Je ne regrette rien... Wake up!"

export N_TASKS=$( wc -l ${FILELIST} | cut -f1 -d' ' )
echo "Number of needed tasks for full statistics of the ${DATASET} dataset: ${N_TASKS}"

