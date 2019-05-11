#! /usr/bin/gnuplot
#
# purpose:
#  generate data reduction graphs for the multi-threaded list project
#
# input: lab2b_list.csv
# 1. test name
# 2. # threads
# 3. # iterations per thread
# 4. # lists
# 5. # operations performed (threads x iterations x (ins + lookup + delete))
# 6. run time (ns)
# 7. run time per operation (ns)
#
# output:
# lab2_list-1.png ... cost per operation vs threads and iterations
# lab2_list-2.png ... threads and iterations that run (un-protected) w/o failure
# lab2_list-3.png ... threads and iterations that run (protected) w/o failure
# lab2_list-4.png ... cost per operation vs number of threads
#
# Note:
# Managing data is simplified by keeping all of the results in a single
# file.  But this means that the individual graphing commands have to
# grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","


# unset the kinky x axis
unset xtics
set xtics

set title "List-1: Throughput of synchronization mechanisms"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Total Number of Operations Per Second"
set logscale y
set output 'lab2b_list-1.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title 'Throughput w/mutex' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[0-9]*,1000,1' lab2b_list.csv" using  ($2):(1000000000/($7)) \
     title 'Throughput w/spin-lock' with linespoints lc rgb 'green'


# unset the kinky x axis
unset xtics
set xtics

set title "List-2: Time Waiting for Locks"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Time (ns) /Operations"
set logscale y
set output 'lab2b_list-2.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1' lab2b_list.csv" using ($2):($8) \
     title 'Average Mutex Wait Time' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[0-9]*,1000,1' lab2b_list.csv" using  ($2):($7) \
     title 'Average Time Per Operation' with linespoints lc rgb 'green'

# unset the kinky x axis
unset xtics
set xtics

set title "List-3: Multiple Lists With and Without Protection"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Successful Iterations"
set logscale y 10
set output 'lab2_list-3.png'
# note that unsuccessful runs should have produced no output
plot \
     "< grep 'list-id-none,[0-9]*,[0-9]*,4' lab2_list.csv" using ($2):($3) \
     title 'no protection' with points lc rgb 'orange', \
     "< grep 'list-id-s,[0-9]*,[0-9]*,4' lab2_list.csv" using ($2):($3) \
     title 'with spinlock' with points lc rgb 'green', \
     "< grep 'list-id-m,[0-9]*,[0-9]*,4' lab2_list.csv" using ($2):($3) \
     title 'with mutex' with points lc rgb 'blue', \

#lab2b_4.png 
# unset the kinky x axis
unset xtics
set xtics

set title "List-4: Throughput with Multiple List and Mutex Lock"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Total Number of Operations Per Second"
set logscale y 10
set output 'lab2b_list-4.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '1 List w/mutex' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[0-9]*,1000,4' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '4 Lists w/mutex' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-m,[0-9]*,1000,8' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '8 Lists w/mutex' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-m,[0-9]*,1000,16' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '16 List w/mutex' with linespoints lc rgb 'orange'

#lab2b_5.png 
# unset the kinky x axis
unset xtics
set xtics

set title "List-4: Throughput with Multiple List and Spin Lock"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Total Number of Operations Per Second"
set logscale y 10
set output 'lab2b_list-5.png'
set key left top
plot \
     "< grep -e 'list-none-s,[0-9]*,1000,1' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '1 List w/mutex' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[0-9]*,1000,4' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '4 Lists w/mutex' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-s,[0-9]*,1000,8' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '8 Lists w/mutex' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-s,[0-9]*,1000,16' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '16 List w/mutex' with linespoints lc rgb 'orange'



