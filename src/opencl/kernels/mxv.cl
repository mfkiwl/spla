#include "common.cl"

void reduction_group(uint                   block_size,
                     uint                   lid,
                     volatile __local TYPE* s_sum) {
    if (BLOCK_SIZE >= block_size) {
        if (lid < (block_size / 2)) {
            s_sum[lid] = OP_BINARY2(s_sum[lid], s_sum[lid + (block_size / 2)]);
        }
        if (block_size > WARP_SIZE) {
            barrier(CLK_LOCAL_MEM_FENCE);
        }
    }
}

__kernel void mxv_vector(__global const uint* g_Ap,
                         __global const uint* g_Aj,
                         __global const TYPE* g_Ax,
                         __global const TYPE* g_vx,
                         __global const TYPE* g_mask,
                         __global TYPE*       g_rx,
                         const TYPE           init,
                         const uint           n) {
    const uint lid     = get_local_id(1);   // thread id in a row
    const uint lsize   = get_local_size(1); // num threads to process row
    const uint lgroup  = get_local_id(0);   // num of rows inside a group
    const uint gid     = get_global_id(0);  // id of row to touch
    const uint gstride = get_global_size(0);// step between row ids

    __local TYPE s_sum[BLOCK_COUNT][BLOCK_SIZE];

    for (int row_id = gid; row_id < n; row_id += gstride) {
        if (lid == 0) {
            g_rx[row_id] = init;
        }

        if (OP_SELECT(g_mask[row_id])) {
            const uint start = g_Ap[row_id];
            const uint end   = g_Ap[row_id + 1];

            TYPE sum = init;

            for (uint i = start + lid; i < end; i += lsize) {
                const uint col_id = g_Aj[i];
                sum               = OP_BINARY2(sum, OP_BINARY1(g_Ax[i], g_vx[col_id]));
            }

            s_sum[lgroup][lid] = sum;
            barrier(CLK_LOCAL_MEM_FENCE);

            reduction_group(64, lid, s_sum[lgroup]);
            reduction_group(32, lid, s_sum[lgroup]);
            reduction_group(16, lid, s_sum[lgroup]);
            reduction_group(8, lid, s_sum[lgroup]);
            reduction_group(4, lid, s_sum[lgroup]);
            reduction_group(2, lid, s_sum[lgroup]);

            if (lid == 0) {
                g_rx[row_id] = s_sum[lgroup][0];
            }
        }
    }
}

__kernel void mxv_scalar(__global const uint* g_Ap,
                         __global const uint* g_Aj,
                         __global const TYPE* g_Ax,
                         __global const TYPE* g_vx,
                         __global const TYPE* g_mask,
                         __global TYPE*       g_rx,
                         const TYPE           init,
                         const uint           n,
                         const uint           early_exit) {
    const uint gid     = get_global_id(0);  // id of row to touch
    const uint gstride = get_global_size(0);// step between row ids

    for (uint row_id = gid; row_id < n; row_id += gstride) {
        TYPE sum = init;

        if (OP_SELECT(g_mask[row_id])) {
            const uint start = g_Ap[row_id];
            const uint end   = g_Ap[row_id + 1];

            for (uint i = start; i < end; i += 1) {
                const uint col_id = g_Aj[i];
                sum               = OP_BINARY2(sum, OP_BINARY1(g_Ax[i], g_vx[col_id]));

                if (early_exit && sum) break;
            }
        }

        g_rx[row_id] = sum;
    }
}