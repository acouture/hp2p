// Copyright (C) 2010-2021 CEA/DAM
// Copyright (C) 2010-2021 Laurent Nguyen <laurent.nguyen@cea.fr>
//
// This file is part of HP2P.
//
// This software is governed by the CeCILL-C license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL-C
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".

/**
 * \file      hp2p_mpi.c
 * \author    Laurent Nguyen <laurent.nguyen@cea.fr>
 * \version   4.0
 * \date      21 June 2021
 * \brief     HP2P Benchmark
 *
 * \details   No
 */

#include "hp2p.h"

int hp2p_mpi_init(int *argc, char ***argv, hp2p_mpi_config *mpi_conf)
{
  int namelen = 0;

  mpi_conf->comm = MPI_COMM_WORLD;
  MPI_Init(argc, argv);
  MPI_Comm_size(mpi_conf->comm, &mpi_conf->nproc);
  MPI_Comm_rank(mpi_conf->comm, &mpi_conf->rank);
  mpi_conf->root = 0;
  MPI_Get_processor_name(mpi_conf->localhost, &namelen);
  mpi_conf->hostlist = malloc(mpi_conf->nproc*MPI_MAX_PROCESSOR_NAME*sizeof(char));
  MPI_Allgather(mpi_conf->localhost, MPI_MAX_PROCESSOR_NAME, MPI_CHAR,
		mpi_conf->hostlist, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, mpi_conf->comm);
  return EXIT_SUCCESS;
};

int hp2p_mpi_get_hostname(hp2p_mpi_config *mpi_conf, int anonymize)
{
  char rank_str[MPI_MAX_PROCESSOR_NAME] = "";

  sprintf(rank_str, "rank_%d", mpi_conf->rank);
  
  if(anonymize == 1)
    {
      MPI_Allgather(rank_str, MPI_MAX_PROCESSOR_NAME, MPI_CHAR,
		    mpi_conf->hostlist, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, mpi_conf->comm);
    }
  return EXIT_SUCCESS;
};

int hp2p_mpi_finalize(hp2p_mpi_config *mpi_conf)
{
  free(mpi_conf->hostlist);
  MPI_Finalize();
  return EXIT_SUCCESS;
};
