
#time mpiexec -n 24 /home/wangzihao/CudaFBP/SingleCuda/Debug/SingleCuda  -M 24 -i BBa_fin.mrc -t BBa_new.tlt  -g -0.3,2,20,400 -o BBa_fbp.mrc -G 2 -m FBP,SheppLogan

#time mpiexec -n 2 valgrind --tool=memcheck --leak-check=yes --show-reachable=yes  /home/wangzihao/CudaFBP/SingleCuda/Debug/SingleCuda  -M 24 -i BBa_fin.mrc -t BBa_new.tlt  -g -0.3,2,20,400 -o BBa_bpt_cuda.mrc -G 2 -m BPT > wzh.bpt 2>&1

#1.
#time mpiexec -n 2 /home/wangzihao/CudaFBP/SingleCuda/Debug/SingleCuda  -M 24 -i BBa_fin.mrc -t BBa_new.tlt  -g     -0.3,2,20,400 -o BBa_bpt_cuda1.mrc -G 2 -m BPT 
#2.
#time mpiexec -n 2 /home/wangzihao/CudaFBP/SingleCuda/Debug/SingleCuda  -M 24 -i BBa_fin.mrc -t BBa_new.tlt  -g -0.3,2,20,400 -o BBa_fbp_cuda.mrc -G 2 -m FBP,SheppLogan

#3.SIRT

#time mpiexec -n 24  /home/wangzihao/CudaFBP/SingleCuda/Debug/SingleCuda -M 24 -i BBa_fin.mrc -n BBa_bpt.mrc -t BBa_new.tlt  -g -0.3,2,20,400 -o BBa_SIRT1_single_cuda.mrc -G 2 -m SIRT,1,0.2 

#4.SART
#time mpiexec -n 2 valgrind --tool=memcheck --leak-check=yes --show-reachable=yes /home/wangzihao/CudaFBP/SingleCuda/Debug/SingleCuda -M 24 -i BBa_fin.mrc -n BBa_bpt.mrc -t BBa_new.tlt  -g -0.3,2,20,400 -o BBa_SIRT1_single_cuda.mrc -G 2 -m SIRT,1,0.2
#5.SART
#time mpiexec -n 2  /home/wangzihao/CudaFBP/SingleCuda/Debug/SingleCuda -M 24 -i BBa_fin.mrc -n BBa_bpt.mrc -t BBa_new.tlt  -g -0.3,2,20,400 -o BBa_SART1_single_cuda.mrc -G 2 -m SART,1,0.2
#6.ASIRT
#time mpiexec -n 2 /home/wangzihao/CudaFBP/SingleCuda/Debug/SingleCuda -M 24 -i BBa_fin.mrc -n BBa_bpt.mrc -t BBa_new.tlt  -g -0.3,2,20,400 -o BBa_ASIRT1_single_cuda.mrc -G 2 -m ASIRT,1,0.2
#7.ASART
time mpiexec -n 2  /HOME/nsfc2015_416/NSFC/wangzihao_data/MPISingle/SingleMpi  -M 24 -i BBa_fin.mrc -n BBa_bpt.mrc -t BBa_new.tlt  -g -0.3,2,20,400 -o BBa_ASART1_single_cuda.mrc -G 2 -m ASART,1,0.2
#time mpiexec -n 2 cuda-memcheck  /home/wangzihao/CudaFBP/SingleCuda/Debug/SingleCuda  -M 24 -i BBa_fin.mrc -t BBa_new.tlt  -g     -0.3,2,20,400 -o BBa_bpt_cuda.mrc -G 2 -m BPT


