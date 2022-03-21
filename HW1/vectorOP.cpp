#include "PPintrin.h"

// implementation of absSerial(), but it is vectorized using PP intrinsics 
void absVector(float *values, float *output, int N) 
{
  __pp_vec_float x;
  __pp_vec_float result;
  __pp_vec_float zero = _pp_vset_float(0.f);
  __pp_mask maskAll, maskIsNegative, maskIsNotNegative;
  // Note: Take a careful look at this loop indexing.  This example
  // code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  // Why is that the case?
  for (int i = 0; i <= N; i += VECTOR_WIDTH)
  {
    // All ones
    maskAll = _pp_init_ones();
    
    // 如果最後不滿VECTOR_WIDTH，修改maskAll
    if (i + VECTOR_WIDTH > N)
    {
      maskAll = _pp_init_ones(N % VECTOR_WIDTH);
    }
    // All zeros
    maskIsNegative = _pp_init_ones(0);
    // Load vector of values from contiguous memory addresses
    _pp_vload_float(x, values + i, maskAll); // x = values[i];
    // Set mask according to predicate
    _pp_vlt_float(maskIsNegative, x, zero, maskAll); // if (x < 0) {
    // Execute instruction using mask ("if" clause)
    _pp_vsub_float(result, zero, x, maskIsNegative); // output[i] = -x;
    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _pp_mask_not(maskIsNegative); // } else {
    
    // Inverse後把多的過濾掉
    maskIsNotNegative = _pp_mask_and(maskIsNotNegative, maskAll);
    // Execute instruction ("else" clause)
    _pp_vload_float(result, values + i, maskIsNotNegative); // output[i] = x; }
    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }
}

void clampedExpVector(float *values, int *exponents, float *output, int N) 
{
  __pp_vec_float x; // 基數
  __pp_vec_int y; // 指數
  __pp_vec_float result; // 答案
  __pp_vec_float maxValue = _pp_vset_float(9.999999f); // 全9.999999
  __pp_vec_int zero = _pp_vset_int(0); // 全0
  __pp_vec_int one = _pp_vset_int(1); // 全1
  __pp_vec_int two = _pp_vset_int(2); // 全2
  __pp_vec_int temp = _pp_vset_int(0); // 暫存
  __pp_mask maskImpl; // 實作標記
  __pp_mask maskNeedImpl; // 需要實作
  __pp_mask maskIsFinish; // 是否完成 (是：1, 否：0)
  __pp_mask maskIsEven; // 是否偶數 (是：1, 否：0)
  __pp_mask maskIsOdd; // 是否奇數 (是：1, 否：0)
  __pp_mask maskIsZero; // exponents為0
  __pp_mask maskTemp; // 暫存
  
  //
  // PP STUDENTS TODO: Implement your vectorized version of
  // clampedExpSerial() here.
  //
  // Your solution should work for any value of
  // N and VECTOR_WIDTH, not just when VECTOR_WIDTH divides N
  //
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    // 初始化result為1
    result = _pp_vset_float(1.f);
    
    // 如果最後不滿VECTOR_WIDTH，maskNeedImpl、maskIsFinish
    if (i + VECTOR_WIDTH > N)
    {
      int M = N % VECTOR_WIDTH;

     // 排除N極小時的陣列使用率過小的問題
      if (N < VECTOR_WIDTH && M < VECTOR_WIDTH / 2)
      {
        for (int j = 0; j < M; j++)
        {
          float rr = 1.0f;
          float bb = values[i + j];
          for (int k = 0; k < exponents[i + j]; k++)
          {
            rr *= bb;
          }

          if (rr > 9.999999f)
          {
            output[i + j] = 9.999999f;
          }
          else
          {
            output[i + j] = rr;
          }
        }
        return;
      }

      maskImpl = _pp_init_ones(M);
      maskNeedImpl = _pp_init_ones(M);
    }
    else
    {
      maskImpl = _pp_init_ones();
      maskNeedImpl = _pp_init_ones();
    }
    
    // All zeros
    maskIsEven = _pp_init_ones(0);
    maskIsZero = _pp_init_ones(0);
    
    // x = values[i];
    _pp_vload_float(x, values + i, maskImpl);
    
    // y = exponents[i];
    _pp_vload_int(y, exponents + i, maskImpl);
    
    // 若 y == 0，加入maskIsFinish並從impl中移除
    _pp_veq_int(maskIsZero, y, zero, maskImpl);
    maskTemp = _pp_mask_not(maskIsZero);
    maskNeedImpl = _pp_mask_and(maskTemp, maskNeedImpl);
    maskIsFinish = _pp_mask_not(maskNeedImpl);
    
    // 如果全部都完成就結束(maskIsFinish == VECTOR_WIDTH)
    do {
      // 判斷exponents奇偶數
      _pp_vdiv_int(temp, y, two, maskNeedImpl);
      _pp_vmult_int(temp, temp, two, maskNeedImpl);
      _pp_veq_int(maskIsEven, y, temp, maskNeedImpl);
      maskIsOdd = _pp_mask_not(maskIsEven);
      
      // 若exponents為奇數，結果乘values
      maskTemp = _pp_mask_and(maskIsOdd, maskNeedImpl);
      _pp_vmult_float(result, result, x, maskTemp);
      
      // 如果exponents為1，標記完成
      _pp_veq_int(maskIsFinish, y, one, maskNeedImpl);
      maskNeedImpl = _pp_mask_not(maskIsFinish);
      
      // values = values * values
      _pp_vmult_float(x, x, x, maskNeedImpl);
      
      // exponents = exponents / 2
      _pp_vdiv_int(y, y, two, maskNeedImpl);
      
    } while (_pp_cntbits(maskIsFinish) != VECTOR_WIDTH);
    
    // 若答案大於9.999999，寫入9.999999
    maskTemp = _pp_init_ones(0);
    _pp_vgt_float(maskTemp, result, maxValue, maskImpl);
    maskTemp = _pp_mask_and(maskTemp, maskImpl);
    _pp_vset_float(result, 9.999999f, maskTemp);
    
    // Write results back to memory
    _pp_vstore_float(output + i, result, maskImpl);
  }
}
// returns the sum of all elements in values 
// You can assume N is a multiple of VECTOR_WIDTH 
// You can assume VECTOR_WIDTH is a power of 2 
float arraySumVector(float *values, int N) 
{
  __pp_vec_float x = _pp_vset_float(0.f);
  __pp_vec_float result = _pp_vset_float(0.f);
  __pp_mask maskAll = _pp_init_ones(); // 全1
  __pp_mask maskAdd; // 要留的
  //
  // PP STUDENTS TODO: Implement your vectorized version of arraySumSerial here
  //
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    
    // x = values[i];
    _pp_vload_float(x, values + i, maskAll);
    
    // result = result + x
    _pp_vadd_float(result, result, x, maskAll);
  }
  
  for (int i = VECTOR_WIDTH; i > 1; i /= 2)
  {
    // 留前面i個值
    maskAdd = _pp_init_ones(i);
    maskAdd = _pp_mask_not(maskAdd);
    _pp_vset_float(result, 0.f, maskAdd);
    
    // 加完放兩邊
    _pp_hadd_float(result, result);
    _pp_interleave_float(result, result);

    if (i % 2 == 1)
    {
      i += 1;
    }
  }
  
  // 合在最前面
  return result.value[0];
}
