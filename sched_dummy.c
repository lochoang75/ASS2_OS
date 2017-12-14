#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 50

/*
 * User defined structure
 */
int timecounter;
 struct sched_atr{
   float throuthput;
   float Avr_turn_around;
   float Avr_waiting_time;
   float Avr_respone_time;
 };
struct sched_atr fcfs;
struct sched_atr sjf;
struct sched_atr srtf;


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
   int turn_around_time;
};

/*
 * Implement scheduling algorithm
 */
//TODO: sort arival time
void sort_arrivaltime(struct process *pl, int pl_size) {
	for (int i = 0; i < pl_size - 1; i++) {
		int min = pl[i].arrivaltime;
		int idx = i;
		for (int j = i + 1; j < pl_size; j++) {
			if (pl[j].arrivaltime < min) {
				min = pl[j].arrivaltime;
				idx = j;
			}
		}
		if(idx != i){
			struct process c = pl[i];
				pl[i] = pl[idx];
				pl[idx] = c;
		}
	}
	return;
}

//TODO: sort vi tri trong file output
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

//TODO: Sort bursttime for sjf
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

//TODO: Implement fcfs function
int sched_fcfs(struct process *pl, int pl_size)
{
   int l = 0;
   fcfs.Avr_turn_around=0;
   fcfs.Avr_waiting_time=0;
   sort_arrivaltime(pl, pl_size);

   /* Do loop simulation engine */
   while (pl_size > 0)
   {
      /* Perform an event */
      if(timecounter >= pl[l].arrivaltime)
      {
         /* TODO: execute that process */
         pl[l].assigned_timeslot[0].starttime = timecounter;
         timecounter += pl[l].bursttime;
         pl[l].assigned_timeslot[0].endtime = timecounter;
         pl[l].turn_around_time=pl[l].assigned_timeslot[0].endtime-pl[l].arrivaltime;
         pl[l].waitingtime=pl[l].assigned_timeslot[0].starttime-pl[l].arrivaltime;
         pl[l].timeslot_count = 1;
         fcfs.Avr_turn_around+=pl[l].turn_around_time;
         fcfs.Avr_waiting_time+=pl[l].waitingtime;
         /* Update a completed task */
         l++;
         pl_size--;
      }else{
         /* Set clock to next event time */
         timecounter++;
      }
   }
   fcfs.Avr_turn_around=fcfs.Avr_turn_around/(float)l;
   fcfs.Avr_waiting_time=fcfs.Avr_waiting_time/(float)l;
   fcfs.throuthput=(float)l/timecounter;

   return 0;
}

//TODO: sched_sjf function
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


		sjf.Avr_waiting_time = 0;
    sjf.Avr_turn_around = 0;
    sjf.Avr_respone_time = 0;

	for(int i = 0; i < pl_size; i++){
		if(pl[i].timeslot_count > 1){
			for(int j = pl[i].timeslot_count; j > 1; j--){
				int k = pl[i].timeslot_count - 1;
				sjf.Avr_waiting_time += (pl[i].assigned_timeslot[j].starttime - pl[i].assigned_timeslot[k].endtime);
			}
			sjf.Avr_waiting_time += (pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime);
      sjf.Avr_turn_around +=pl[i].assigned_timeslot[pl[i].timeslot_count-1].endtime-pl[i].arrivaltime;
      sjf.Avr_respone_time+=pl[i].assigned_timeslot[0].starttime-pl[i].arrivaltime;
		}
		else if(pl[i].timeslot_count == 1){
       sjf.Avr_waiting_time += (pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime);
       sjf.Avr_turn_around +=pl[i].assigned_timeslot[0].endtime-pl[i].arrivaltime;
       sjf.Avr_respone_time+=pl[i].assigned_timeslot[0].starttime-pl[i].arrivaltime;
     }
	}
  sjf.throuthput=(float)pl_size/timecounter;
  sjf.Avr_waiting_time=(float)sjf.Avr_waiting_time/pl_size;
  sjf.Avr_turn_around=(float)sjf.Avr_turn_around/pl_size;
  sjf.Avr_respone_time=(float)sjf.Avr_respone_time/pl_size;
	return 0;
}

//Implement SRTF function
int sched_SRTF(struct process *pl, int pl_size)
{
	timecounter =0;
	int l = 0;
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
							check = 0;
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
  srtf.Avr_waiting_time = 0;
  srtf.Avr_turn_around = 0;
  srtf.Avr_respone_time = 0;

for(int i = 0; i < pl_size; i++){
  if(pl[i].timeslot_count > 1){
    for(int j = pl[i].timeslot_count; j > 1; j--){
      int k = j - 1;
      srtf.Avr_waiting_time += (pl[i].assigned_timeslot[j-1].starttime - pl[i].assigned_timeslot[j-2].endtime);
    }
    srtf.Avr_waiting_time += (pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime);
    srtf.Avr_turn_around +=pl[i].assigned_timeslot[pl[i].timeslot_count-1].endtime-pl[i].arrivaltime;
    srtf.Avr_respone_time+=pl[i].assigned_timeslot[0].starttime-pl[i].arrivaltime;
  }
  else if(pl[i].timeslot_count == 1){
     srtf.Avr_waiting_time += (pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime);
     srtf.Avr_turn_around +=pl[i].assigned_timeslot[0].endtime-pl[i].arrivaltime;
     srtf.Avr_respone_time+=pl[i].assigned_timeslot[0].starttime-pl[i].arrivaltime;
   }
}
srtf.throuthput=(float)pl_size/timecounter;
srtf.Avr_waiting_time=(float)srtf.Avr_waiting_time/pl_size;
srtf.Avr_turn_around=(float)srtf.Avr_turn_around/pl_size;
srtf.Avr_respone_time=(float)srtf.Avr_respone_time/pl_size;
return 0;
}

//Implement round robin
int sched_rr(struct process *pl, int pl_size, int qtime)
{
	for (int k = 0; k < pl_size; k++) {
		pl[k].flag = k;
	}
	int l = 0;
	sort_arrivaltime(pl, pl_size);
	while (l < pl_size) {
		if (pl[l].arrivaltime <= timecounter ) {
			if (pl[l].bursttime > qtime) {
				pl[l].assigned_timeslot[pl[l].timeslot_count].starttime = timecounter;
				timecounter += qtime;
				pl[l].assigned_timeslot[pl[l].timeslot_count].endtime = timecounter;
				pl[l].timeslot_count++;
				pl[l].bursttime = pl[l].bursttime - qtime;
				pl[l].arrivaltime = timecounter;
				int idx = pl[l].flag;
				pl[l].arrivaltime ++;
				sort_arrivaltime(pl, pl_size);
				for(int r = l; r<pl_size; r++){
					if(pl[r].flag == idx) {pl[r].arrivaltime--;}
				}
			}
			else {
				pl[l].assigned_timeslot[pl[l].timeslot_count].starttime = timecounter;
				timecounter += pl[l].bursttime;
				pl[l].assigned_timeslot[pl[l].timeslot_count].endtime = timecounter;
				pl[l].timeslot_count++;
				pl[l].bursttime = 0;
				pl[l].arrivaltime = 0;
				l++;
				sort_arrivaltime(pl, pl_size);
			}


		}
		else timecounter++;
	}
	for (int y = 0; y < pl_size-1; y++) {
		int min = pl[y].flag;
		for (int e = y + 1; e < pl_size; e++) {
			if (min > pl[e].flag) {
				min = pl[e].flag;
				struct process c = pl[y];
				pl[y] = pl[e];
				pl[e] = c;
			}
		}
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
   struct process pl2[MAX_PROCESS];
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
   //copy bursttime to save bursttime after sjf and SRTF
   for(int i=0;i<pl_size;i++){
     pl2[i].arrivaltime=pl[i].arrivaltime;
     pl2[i].bursttime=pl[i].bursttime;
   }

   /* Implement your scheduler */


   l=0;
   ofp = fopen(ovalue, "w");
   if(ofp == NULL){
      perror ("ERROR: output file open failed\n");
      return (-1);
   }
   sched_fcfs(pl,pl_size);
   timecounter=0;
   for(int i=0;i<pl_size;i++){
     free(pl[i].assigned_timeslot);
     pl[i].assigned_timeslot = malloc(sizeof(struct timeslot) * 10); // assume the maximum number of time_slot is 10
     pl[i].timeslot_count = 0;
     pl[i].waitingtime=0;
     pl[i].timeslot_count;
     pl[i].flag;
     pl[i].vitri;
     pl[i].waitingtime;
     pl[i].turn_around_time;
   }

   sched_sjf(pl,pl_size);
   timecounter=0;
   for(int i=0;i<pl_size;i++){
     pl2[i].assigned_timeslot = malloc(sizeof(struct timeslot) * 10); // assume the maximum number of time_slot is 10
     pl2[i].timeslot_count = 0;
     pl2[i].waitingtime=0;
     pl2[i].timeslot_count=0;
     pl2[i].flag=0;
     pl2[i].vitri=0;
     pl2[i].waitingtime=0;
     pl2[i].turn_around_time=0;
   }
   sched_SRTF(pl2,pl_size);
   /* Produce output data */
   fprintf(ofp,"**************************************************************************\n" );
   fprintf(ofp,"*                         *%s    *%s        *%s \n"," FCFS "," SJF "," SRTF " );
   fprintf(ofp,"**************************************************************************\n" );
   fprintf(ofp,"* Throuthput               * %f * %f   * %f\n",fcfs.throuthput,sjf.throuthput,srtf.throuthput);
   fprintf(ofp,"**************************************************************************\n" );
   fprintf(ofp,"* Average turn around time * %f * %f   * %f\n",fcfs.Avr_turn_around,sjf.Avr_turn_around,srtf.Avr_turn_around);
   fprintf(ofp,"**************************************************************************\n" );
   fprintf(ofp,"* Average waitingtime      * %f * %f   * %f\n",fcfs.Avr_waiting_time,sjf.Avr_waiting_time,srtf.Avr_waiting_time);
   fprintf(ofp,"**************************************************************************\n" );
   fprintf(ofp,"* Average respone time     * %f * %f   * %f\n",fcfs.Avr_waiting_time,sjf.Avr_respone_time,srtf.Avr_waiting_time);
   fprintf(ofp,"**************************************************************************\n" );
   while (l < pl_size)
   {
      for(i = 0; i < pl2[l].timeslot_count; i++)
         fprintf(ofp, "Process %2d start %5d end %5d \n", l,
                  pl2[l].assigned_timeslot[i].starttime, pl2[l].assigned_timeslot[i].endtime);
      l++;
   }
   return 0;
}
