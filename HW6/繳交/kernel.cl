__kernel void convolution(int filterWidth, __constant float *filter, int imageHeight, int imageWidth, __global float *inputImage, __global float *outputImage) 
{
    int x, xe, y, l, yl, index, temp_x, temp_f, temp_i;
    float4 sum, sub_sum;
    
    index = get_global_id(0) << 2;
    x = index / imageWidth - filterWidth;
    xe = index / imageWidth + filterWidth;
    y = index % imageWidth;

    sum = (float4) 0.0f;
    temp_f = 0;
    temp_x = x * imageWidth;
    for (; x <= xe; x++, temp_x += imageWidth)
    {
        for (l = -filterWidth; l <= filterWidth; l++, temp_f++)
        {
            if (filter[temp_f] == 0) continue;
            if (x < 4 || x + 4 >= imageHeight) continue;
            
            yl = y + l;
            if (yl < 4 || yl + 4 >= imageWidth) continue;
            
			temp_i = temp_x + yl;
            // sub_sum = vload4(0, &inputImage[temp_x + yl]);
			sub_sum = (float4) (inputImage[temp_i], inputImage[temp_i + 1],inputImage[temp_i + 2], inputImage[temp_i + 3]);
            sub_sum *= filter[temp_f];
            
            sum += sub_sum;
        }
    }
    // vstore4(sum, 0, &outputImage[index]);
    outputImage[index++] = sum.x;
    outputImage[index++] = sum.y;
    outputImage[index++] = sum.z;
    outputImage[index] = sum.w;
}
