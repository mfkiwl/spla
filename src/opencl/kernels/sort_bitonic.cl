#include "common.cl"

#define SWAP_KEYS(x, y) \
    uint tmp1 = x;      \
    x         = y;      \
    y         = tmp1;

#define SWAP_VALUES(x, y) \
    TYPE tmp2 = x;        \
    x         = y;        \
    y         = tmp2;

uint ceil_to_pow2(uint n) {
    uint r = 1;
    while (r < n) r *= 2;
    return r;
}

__kernel void bitonic_sort_local(__global uint* g_keys,
                                 __global TYPE* g_values,
                                 const uint     n) {
    const uint lid       = get_local_id(0);
    const uint lsize     = get_local_size(0);
    const uint n_aligned = ceil_to_pow2(n);
    const uint n_threads = n_aligned / 2;

    __local uint s_keys[BITONIC_SORT_LOCAL_BUFFER_SIZE];
    __local uint s_values[BITONIC_SORT_LOCAL_BUFFER_SIZE];

    for (uint i = lid; i < n; i += lsize) {
        s_keys[i]   = g_keys[i];
        s_values[i] = g_values[i];
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    for (uint segment_size = 2; segment_size <= n_aligned; segment_size *= 2) {
        const uint segment_size_half = segment_size / 2;

        for (uint tid = lid; tid < n_threads; tid += lsize) {
            const uint segment_id       = tid / segment_size_half;
            const uint inner_id         = tid % segment_size_half;
            const uint inner_id_sibling = segment_size - inner_id - 1;
            const uint i                = segment_id * segment_size + inner_id;
            const uint j                = segment_id * segment_size + inner_id_sibling;

            if (i < n && j < n && s_keys[i] > s_keys[j]) {
                SWAP_KEYS(s_keys[i], s_keys[j]);
                SWAP_VALUES(s_values[i], s_values[j]);
            }
        }

        barrier(CLK_LOCAL_MEM_FENCE);

        for (uint k = segment_size_half / 2; k > 0; k /= 2) {
            for (uint tid = lid; tid < n_threads; tid += lsize) {
                const uint segment_size_inner = k * 2;
                const uint segment_id         = tid / k;
                const uint inner_id           = tid % k;
                const uint inner_id_sibling   = inner_id + k;
                const uint i                  = segment_id * segment_size_inner + inner_id;
                const uint j                  = segment_id * segment_size_inner + inner_id_sibling;

                if (i < n && j < n && s_keys[i] > s_keys[j]) {
                    SWAP_KEYS(s_keys[i], s_keys[j]);
                    SWAP_VALUES(s_values[i], s_values[j]);
                }
            }

            barrier(CLK_LOCAL_MEM_FENCE);
        }
    }

    for (uint i = lid; i < n; i += lsize) {
        g_keys[i]   = s_keys[i];
        g_values[i] = s_values[i];
    }
}

__kernel void bitonic_sort_global(__global uint* g_keys,
                                  __global TYPE* g_values,
                                  const uint     n) {
    const uint lid       = get_local_id(0);
    const uint lsize     = get_local_size(0);
    const uint n_aligned = ceil_to_pow2(n);
    const uint n_threads = n_aligned / 2;

    for (uint segment_size = 2; segment_size <= n_aligned; segment_size *= 2) {
        const uint segment_size_half = segment_size / 2;

        for (uint tid = lid; tid < n_threads; tid += lsize) {
            const uint segment_id       = tid / segment_size_half;
            const uint inner_id         = tid % segment_size_half;
            const uint inner_id_sibling = segment_size - inner_id - 1;
            const uint i                = segment_id * segment_size + inner_id;
            const uint j                = segment_id * segment_size + inner_id_sibling;

            if (i < n && j < n && g_keys[i] > g_keys[j]) {
                SWAP_KEYS(g_keys[i], g_keys[j]);
                SWAP_VALUES(g_values[i], g_values[j]);
            }
        }

        barrier(CLK_GLOBAL_MEM_FENCE);

        for (uint k = segment_size_half / 2; k > 0; k /= 2) {
            for (uint tid = lid; tid < n_threads; tid += lsize) {
                const uint segment_size_inner = k * 2;
                const uint segment_id         = tid / k;
                const uint inner_id           = tid % k;
                const uint inner_id_sibling   = inner_id + k;
                const uint i                  = segment_id * segment_size_inner + inner_id;
                const uint j                  = segment_id * segment_size_inner + inner_id_sibling;

                if (i < n && j < n && g_keys[i] > g_keys[j]) {
                    SWAP_KEYS(g_keys[i], g_keys[j]);
                    SWAP_VALUES(g_values[i], g_values[j]);
                }
            }

            barrier(CLK_GLOBAL_MEM_FENCE);
        }
    }
}