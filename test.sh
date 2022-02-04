#!/bin/bash
gcc adsb_R4.c -o r4_test -O2 -lm

#ここからModel1の各データの実行時間
echo --Model1/dat0-- > result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 27817,15689 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model1/dat0/idata" "Model1/dat0/odata") >> result.txt 2>&1

echo "" >> result.txt
echo --Model1/dat1-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 30029,15710 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model1/dat1/idata" "Model1/dat1/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model1/dat2-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 32787,16740 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model1/dat2/idata" "Model1/dat2/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model1/dat3-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 9857,15684 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model1/dat3/idata" "Model1/dat3/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model1/dat4-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 20310,15715 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model1/dat4/idata" "Model1/dat4/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model1/dat5-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 39154,15527 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model1/dat5/idata" "Model1/dat5/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model1/dat6-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 41883,16527 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model1/dat6/idata" "Model1/dat6/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model1/dat7-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 18369,15440 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model1/dat7/idata" "Model1/dat7/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model1/dat8-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 43868,15754 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model1/dat8/idata" "Model1/dat8/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model1/dat9-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 36994,15545 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model1/dat9/idata" "Model1/dat9/odata") >> result.txt 2>&1



#ここからModel2の各データの実行時間
echo "" >> result.txt
echo --Model2/dat0-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 29771,34379 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model2/dat0/idata" "Model2/dat0/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model2/dat1-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 40856,34632 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model2/dat1/idata" "Model2/dat1/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model2/dat2-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 36306,32389 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model2/dat2/idata" "Model2/dat2/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model2/dat3-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 45594,35685 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model2/dat3/idata" "Model2/dat3/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model2/dat4-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 11043,33470 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model2/dat4/idata" "Model2/dat4/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model2/dat5-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 17419,31179 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model2/dat5/idata" "Model2/dat5/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model2/dat6-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 30327,34967 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model2/dat6/idata" "Model2/dat6/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model2/dat7-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 34045,34813 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model2/dat7/idata" "Model2/dat7/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model2/dat8-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 16468,31148 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model2/dat8/idata" "Model2/dat8/odata") >> result.txt 2>&1


echo "" >> result.txt
echo --Model2/dat9-- >> result.txt
echo "" >> result.txt
echo "pos,err(answer)" >> result.txt
echo 39154,33147 >> result.txt
echo "pos,err(group)" >> result.txt
(time ./r4_test "Model2/dat9/idata" "Model2/dat9/odata") >> result.txt 2>&1



#ここからはModel1の平均実行時間
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
echo "" >> result.txt
echo "Model1の平均実行時間" >> result.txt
bc <<< "scale=4; $run_time/10000" >> result.txt

#ここからはModel1の平均実行時間
start_time=`date +%s%3N`
 
### 計測したい処理・コマンド ###
./r4_test "Model2/dat0/idata" "Model2/dat0/odata"
./r4_test "Model2/dat1/idata" "Model2/dat1/odata"
./r4_test "Model2/dat2/idata" "Model2/dat2/odata"
./r4_test "Model2/dat3/idata" "Model2/dat3/odata"
./r4_test "Model2/dat4/idata" "Model2/dat4/odata"
./r4_test "Model2/dat5/idata" "Model2/dat5/odata"
./r4_test "Model2/dat6/idata" "Model2/dat6/odata"
./r4_test "Model2/dat7/idata" "Model2/dat7/odata"
./r4_test "Model2/dat8/idata" "Model2/dat8/odata"
./r4_test "Model2/dat9/idata" "Model2/dat9/odata"
 
end_time=`date +%s%3N`
 
run_time=$((end_time - start_time))
echo "" >> result.txt
echo "Model2の平均実行時間" >> result.txt
bc <<< "scale=4; $run_time/10000" >> result.txt

