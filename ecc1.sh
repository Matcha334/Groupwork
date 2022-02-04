#!/bin/bash
gcc nprep_norev.c -o r4_test

start_time=`date +%s%3N`
 
### 計測したい処理・コマンド ###
./r4_test "Model1/dat0/idata" "Model1/dat0/odata"
./r4_test "Model1/dat1/idata" "Model1/dat1/odata"
./r4_test "Model1/dat2/idata" "Model1/dat2/odata"
./r4_test "Model1/dat3/idata" "Model1/dat3/odata"
./r4_test "Model1/dat4/idata" "Model1/dat4/odata"
./r4_test "Model1/dat5/idata" "Model1/dat5/odata"
./r4_test "Model1/dat6/idata" "Model1/dat6/odata"
./r4_test "Model1/dat7/idata" "Model1/dat7/odata"
./r4_test "Model1/dat8/idata" "Model1/dat8/odata"
./r4_test "Model1/dat9/idata" "Model1/dat9/odata"
 
end_time=`date +%s%3N`
 
run_time=$((end_time - start_time))
echo "1データセットあたりの平均実行時間"
bc <<< "scale=4; $run_time/10000"


