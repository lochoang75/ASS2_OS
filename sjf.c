#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 50

int timecounter = 0;

/*
 * User defined structure
 */

struct timeslot
{
   int starttime; /* Timestamp at the start of execution */
   int endtime;   /* Timestamp at the   end of execution */
};

struct process
{
   /* Values initialized for each process */
   int arrivaltime;  /* Timestamp process arrives and
                        wish to start */
   int bursttime;    /* An amount of time process 
                        requires to complete the job */

   /* Values algorithm may use to track process */
   struct timeslot* assigned_timeslot;
   int timeslot_count;
   int flag;
   int vitri;
   int waitingtime;
};

/*
 * Implement scheduling algorithm
 */
void sort_bursttime(struct process *pl, int pl_size) {
	for (int i = 0; i < pl_size - 1; i++) {
		int min = pl[i].bursttime;
		for (int j = i + 1; j < pl_size; j++) {
			if (pl[j].bursttime < min) {
				min = pl[j].bursttime;
				struct process c = pl[i];
				pl[i] = pl[j];
				pl[j] = c;
			}
		}
	}
	return;
}

void sort_vitri(struct process *pl, int pl_size) {
	for (int i = 0; i < pl_size - 1; i++) {
		int min = pl[i].vitri;
		for (int j = i + 1; j < pl_size; j++) {
			if (pl[j].vitri < min) {
				min = pl[j].vitri;
				struct process c = pl[i];
				pl[i] = pl[j];
				pl[j] = c;
			}
		}
	}
	return;
}

int sched_sjf(struct process *pl, int pl_size)
{ 
	timecounter =0;
	int l = 0;
	for(int j = 0; j < pl_size; j++){
		pl[j].vitri=j;
	}

	/* TODO: sort arrival time */
	//sort_on_arrival_time(pl, pl_szie);
	sort_bursttime(pl, pl_size);
	/* Do loop simulation engine */
	while ( l < (pl_size))
	{
		/* Perfo
		rm an event */
		if (timecounter >= pl[l].arrivaltime)
		{
			/* TODO: execute that process */
				pl[l].assigned_timeslot[pl[l].timeslot_count].starttime = timecounter;
				timecounter += pl[l].bursttime;
				pl[l].assigned_timeslot[pl[l].timeslot_count].endtime = timecounter;
				pl[l].timeslot_count += 1;
				pl[l].bursttime =0;
				l += 1;
			/* Update a completed task */
		}
		else {
			int i = l;
			for (int z = l + 1; z < pl_size; z++) {
				if (timecounter >= pl[z].arrivaltime) {
					i = z;
					break;
				}
			}
			if (i != l) {
				int check = 1;
				pl[i].assigned_timeslot[pl[i].timeslot_count].starttime = timecounter;
				while (check == 1) {
					timecounter++;
					pl[i].bursttime--;
					for (int a = l; a < i; a++) {
						if (pl[a].arrivaltime <= timecounter && pl[a].bursttime < pl[i].bursttime) {
							check = 1;
							break;
						}
						else check = 1;
					}
					if (pl[i].bursttime < 1) { check = 0; l++; }
				}
				pl[i].assigned_timeslot[pl[i].timeslot_count].endtime = timecounter;
				pl[i].timeslot_count += 1;
				sort_bursttime(pl, pl_size);
			}
		   else timecounter++; 
		} 	
	
	}
	sort_vitri(pl,pl_size);
	
	for(int i = 0; i < pl_size; i++){
		pl[i].waitingtime = 0;
	}
	
	for(int i = 0; i < pl_size; i++){
		if(pl[i].timeslot_count > 1){
			for(int j = pl[i].timeslot_count; j > 1; j--){
				int k = pl[i].timeslot_count - 1;
				pl[i].waitingtime += (pl[i].assigned_timeslot[j].starttime - pl[i].assigned_timeslot[k].endtime);
			}
			pl[i].waitingtime += (pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime);
		}
		else if(pl[i].timeslot_count == 1) pl[i].waitingtime += (pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime);
	}

	return 0;
}

/*
 * Main routine
 */

int main(int argc, char* argv[])
{
   char *ivalue = NULL;
   char *ovalue = NULL;
   char *qvalue = NULL;
   FILE *ifp, *ofp;
   struct process pl[MAX_PROCESS];
   int pl_size=0;
   int c,l;
   int quantum_time=1;
   char* endptr;
   int i;

   /* Program argument parsing */
   while ((c = getopt (argc, argv, "i:o:p:")) != -1)
      switch (c)
         {
            case 'i':
               ivalue = optarg;
               break;
            case 'o':
               ovalue = optarg;
               break;
            case 'p':
               qvalue = optarg;
               break;
         }

   /* Validate input arguments */
   if (ivalue == NULL){
      perror ("WARNING: use default input file");
      ivalue = "input.txt";
   }

   if (ovalue == NULL){
      perror ("WARNING: use default output file");
      ovalue = "output.txt";
   }

   if (qvalue != NULL){
      quantum_time = (int) strtol(qvalue, &endptr, 10); //base decimal
      if ( quantum_time < 1 || *endptr) {
          fprintf(stderr, "ERROR: Invalid quantum time\n");
          return (-1);
      }
      fprintf(stderr, "INFO: use passing argument quantum_time %d\n", quantum_time);
   }

   /* Get data input */
   ifp = fopen(ivalue, "r");
   if(ifp == NULL){
      perror ("ERROR: input file open failed\n");
      return (-1);
   }

   l = 0;
   while(fscanf(ifp,"%d %d",&pl[l].arrivaltime,&pl[l].bursttime) != EOF){
      /* Initialize the instance of struct process before using */
      pl[l].assigned_timeslot = malloc(sizeof(struct timeslot) * 10); // assume the maximum number of time_slot is 10
      pl[l].timeslot_count = 0;

      /* ACKnowledge a new process has been imported */
      pl_size = ++l;
   }

   /* Initialize system clock */
   timecounter = 0;

   /* Implement your scheduler */
// sched_dummy(pl, pl_size);
//   sched_fcfs(pl, pl_size);
   sched_sjf(pl, pl_size);

   l=0;
   ofp = fopen(ovalue, "w");
   if(ofp == NULL){
      perror ("ERROR: output file open failed\n");
      return (-1);
   }

   /* Produce output data */
   while (l < pl_size)
   {
      for(i = 0; i < pl[l].timeslot_count; i++)
         fprintf(ofp, "Process %2d start %5d end %5d waiting time %5d\n", l, 
                  pl[l].assigned_timeslot[i].starttime, pl[l].assigned_timeslot[i].endtime, pl[l].waitingtime);
      l++;
   }

   /* TODO: calculate scheduling criteria 
            base on pl (=process_list) which is already existed */

   return 0;
}
