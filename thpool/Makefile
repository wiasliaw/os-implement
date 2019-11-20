CC:=gcc
EXP=example
jobq=jobqueue
pool=thpool

example:
	$(CC) $(EXP).c $(pool).c  ../$(jobq)/$(jobq).c -lpthread -o $(EXP)

clean:
	rm -rf $(EXP)
