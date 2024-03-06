myfs.c is configured to be able to run from the lab machines.
It currently uses /tmp/matthewh on both the client and server, although this can be modified to any tmp directory.
To compile, run `make; make mount`.
Make sure that /tmp/matthewh exists on pegasus before running and that the files you want to access also exist on pegasus.

To use myfs without the lab machines, you will need to set up ssh keys and use myfs_local.c.
You can update it to point to your server of choice.