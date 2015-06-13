/**************************************************************************************************
*                                                                                                 *
* This file is part of HPMPC.                                                                     *
*                                                                                                 *
* HPMPC -- Library for High-Performance implementation of solvers for MPC.                        *
* Copyright (C) 2014-2015 by Technical University of Denmark. All rights reserved.                *
*                                                                                                 *
* HPMPC is free software; you can redistribute it and/or                                          *
* modify it under the terms of the GNU Lesser General Public                                      *
* License as published by the Free Software Foundation; either                                    *
* version 2.1 of the License, or (at your option) any later version.                              *
*                                                                                                 *
* HPMPC is distributed in the hope that it will be useful,                                        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                                            *
* See the GNU Lesser General Public License for more details.                                     *
*                                                                                                 *
* You should have received a copy of the GNU Lesser General Public                                *
* License along with HPMPC; if not, write to the Free Software                                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA                  *
*                                                                                                 *
* Author: Gianluca Frison, giaf (at) dtu.dk                                                       *
*                                                                                                 *
**************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "../include/aux_d.h"
/*#include "../include/aux_s.h"*/
#include "../include/blas_d.h"
/*#include "../include/blas_lib_s.h"*/
#include "../include/block_size.h"
#include "test_param.h"



//void dgemm_(char *ta, char *tb, int *m, int *n, int *k, double *alpha, double *A, int *lda, double *B, int *ldb, double *beta, double *C, int *ldc);
//void dgemv_(char *ta, int *m, int *n, double *alpha, double *A, int *lda, double *x, int *incx, double *beta, double *y, int *incy);

#if defined(REF_BLAS_OPENBLAS)
#include "../reference_code/blas.h"
void openblas_set_num_threads(int n_thread);
#endif
#if defined(REF_BLAS_BLIS)
#include <blis/blis.h>
#endif
#if defined(REF_BLAS_NETLIB)
#include "../reference_code/blas.h"
#endif



int main()
	{
		
#if defined(REF_BLAS_OPENBLAS)
	openblas_set_num_threads(1);
#endif
#if defined(REF_BLAS_BLIS)
	omp_set_num_threads(1);
#endif

	printf("\n");
	printf("\n");
	printf("\n");
	printf(" HPMPC -- Library for High-Performance implementation of solvers for MPC.\n");
	printf(" Copyright (C) 2014-2015 by Technical University of Denmark. All rights reserved.\n");
	printf("\n");
	printf(" HPMPC is distributed in the hope that it will be useful,\n");
	printf(" but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	printf(" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
	printf(" See the GNU Lesser General Public License for more details.\n");
	printf("\n");
	printf("\n");
	printf("\n");

	printf("BLAS performance test - double precision\n");
	printf("\n");

	// maximum frequency of the processor
	const float GHz_max = GHZ_MAX;
	printf("Frequency used to compute theoretical peak: %5.1f GHz (edit test_param.h to modify this value).\n", GHz_max);
	printf("\n");

	// maximum flops per cycle, double precision
#if defined(TARGET_X64_AVX2)
	const float flops_max = 16;
	printf("Testing BLAS version for AVX2 & FMA3 instruction sets, 64 bit: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#elif defined(TARGET_X64_AVX)
	const float flops_max = 8;
	printf("Testing BLAS version for AVX instruction set, 64 bit: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#elif defined(TARGET_X64_SSE3) || defined(TARGET_AMD_SSE3)
	const float flops_max = 4;
	printf("Testing BLAS version for SSE3 instruction set, 64 bit: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#elif defined(TARGET_CORTEX_A15)
	const float flops_max = 2;
	printf("Testing solvers for ARMv7a VFPv3 instruction set, oprimized for Cortex A15: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#elif defined(TARGET_CORTEX_A9)
	const float flops_max = 1;
	printf("Testing solvers for ARMv7a VFPv3 instruction set, oprimized for Cortex A9: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#elif defined(TARGET_CORTEX_A7)
	const float flops_max = 0.5;
	printf("Testing solvers for ARMv7a VFPv3 instruction set, oprimized for Cortex A7: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#elif defined(TARGET_X86_ATOM)
	const float flops_max = 1;
	printf("Testing BLAS version for SSE3 instruction set, 32 bit, optimized for Intel Atom: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#elif defined(TARGET_POWERPC_G2)
	const float flops_max = 1;
	printf("Testing BLAS version for POWERPC instruction set, 32 bit: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#elif defined(TARGET_C99_4X4)
	const float flops_max = 2;
	printf("Testing reference BLAS version, 4x4 kernel: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#elif defined(TARGET_C99_4X4_PREFETCH)
	const float flops_max = 2;
	printf("Testing reference BLAS version, 4x4 kernel with register prefetch: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#elif defined(TARGET_C99_2X2)
	const float flops_max = 2;
	printf("Testing reference BLAS version, 2x2 kernel: theoretical peak %5.1f Gflops\n", flops_max*GHz_max);
#endif
	
	FILE *f;
	f = fopen("./test_problems/results/test_blas.m", "w"); // a

#if defined(TARGET_X64_AVX2)
	fprintf(f, "C = 'd_x64_avx2';\n");
	fprintf(f, "\n");
#elif defined(TARGET_X64_AVX)
	fprintf(f, "C = 'd_x64_avx';\n");
	fprintf(f, "\n");
#elif defined(TARGET_X64_SSE3) || defined(TARGET_AMD_SSE3)
	fprintf(f, "C = 'd_x64_sse3';\n");
	fprintf(f, "\n");
#elif defined(TARGET_CORTEX_A9)
	fprintf(f, "C = 'd_ARM_cortex_A9';\n");
	fprintf(f, "\n");
#elif defined(TARGET_CORTEX_A7)
	fprintf(f, "C = 'd_ARM_cortex_A7';\n");
	fprintf(f, "\n");
#elif defined(TARGET_CORTEX_A15)
	fprintf(f, "C = 'd_ARM_cortex_A15';\n");
	fprintf(f, "\n");
#elif defined(TARGET_X86_ATOM)
	fprintf(f, "C = 'd_x86_atom';\n");
	fprintf(f, "\n");
#elif defined(TARGET_POWERPC_G2)
	fprintf(f, "C = 'd_PowerPC_G2';\n");
	fprintf(f, "\n");
#elif defined(TARGET_C99_4X4)
	fprintf(f, "C = 'd_c99_4x4';\n");
	fprintf(f, "\n");
#elif defined(TARGET_C99_4X4_PREFETCH)
	fprintf(f, "C = 'd_c99_4x4';\n");
	fprintf(f, "\n");
#elif defined(TARGET_C99_2X2)
	fprintf(f, "C = 'd_c99_2x2';\n");
	fprintf(f, "\n");
#endif

	fprintf(f, "A = [%f %f];\n", GHz_max, flops_max);
	fprintf(f, "\n");

	fprintf(f, "B = [\n");
	


	int i, j, rep, ll;
	
	const int bsd = D_MR; //d_get_mr();

/*	int info = 0;*/
	
	printf("\nn\t  kernel_dgemm\t  dgemm\t\t  dsyrk_dpotrf\t  dtrmm\t\t  dtrtr\t\t  dgemv_n\t  dgemv_t\t  dtrmv_n\t  dtrmv_t\t  dtrsv_n\t  dtrsv_t\t  dsymv\t\t  dmvmv\t\t  dsyrk+dpotrf\t  BLAS dgemm\t  BLAS dgemv_n\t  BLAS dgemv_t\n");
	printf("\nn\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\t Gflops\t    %%\n\n");
	
	int nn[] = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 256, 260, 264, 268, 272, 276, 280, 284, 288, 292, 296, 300, 304, 308, 312, 316, 320, 324, 328, 332, 336, 340, 344, 348, 352, 356, 360, 364, 368, 372, 376, 380, 384, 388, 392, 396, 400, 404, 408, 412, 416, 420, 424, 428, 432, 436, 440, 444, 448, 452, 456, 460, 500, 550, 600, 650, 700};
	int nnrep[] = {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 400, 400, 400, 400, 400, 200, 200, 200, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 20, 20, 20, 20, 20, 20, 20, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 4, 4, 4, 4, 4};
	
	for(ll=0; ll<75; ll++)
//	for(ll=0; ll<115; ll++)
//	for(ll=0; ll<120; ll++)

		{

		int n = nn[ll];
		int nrep = nnrep[ll];

#if defined(REF_BLAS_BLIS)
		f77_int n77 = n;
#endif
	
		double *A; d_zeros(&A, n, n);
		double *B; d_zeros(&B, n, n);
		double *C; d_zeros(&C, n, n);
		double *M; d_zeros(&M, n, n);

		char c_n = 'n';
		char c_t = 't';
		int i_1 = 1;
#if defined(REF_BLAS_BLIS)
		f77_int i77_1 = i_1;
#endif
		double d_1 = 1;
		double d_0 = 0;
	
		for(i=0; i<n*n; i++)
			A[i] = i;
	
		for(i=0; i<n; i++)
			B[i*(n+1)] = 1;
	
		for(i=0; i<n*n; i++)
			M[i] = 1;
	
		int pnd = ((n+bsd-1)/bsd)*bsd;	
		int cnd = ((n+D_NCL-1)/D_NCL)*D_NCL;	
		int cnd2 = 2*((n+D_NCL-1)/D_NCL)*D_NCL;	
		int pad = (D_NCL-n%D_NCL)%D_NCL;

		double *pA; d_zeros_align(&pA, pnd, cnd);
		double *pB; d_zeros_align(&pB, pnd, cnd);
		double *pC; d_zeros_align(&pC, pnd, cnd);
		double *pD; d_zeros_align(&pD, pnd, cnd);
		double *pE; d_zeros_align(&pE, pnd, cnd2);
		double *pF; d_zeros_align(&pF, 2*pnd, cnd);
		double *pL; d_zeros_align(&pL, pnd, cnd);
		double *pM; d_zeros_align(&pM, pnd, cnd);
		double *x; d_zeros_align(&x, pnd, 1);
		double *y; d_zeros_align(&y, pnd, 1);
		double *x2; d_zeros_align(&x2, pnd, 1);
		double *y2; d_zeros_align(&y2, pnd, 1);
		double *diag; d_zeros_align(&diag, pnd, 1);
	
		d_cvt_mat2pmat(n, n, A, n, 0, pA, cnd);
		d_cvt_mat2pmat(n, n, B, n, 0, pB, cnd);
		d_cvt_mat2pmat(n, n, B, n, 0, pD, cnd);
		d_cvt_mat2pmat(n, n, A, n, 0, pE, cnd2);
		d_cvt_mat2pmat(n, n, M, n, 0, pM, cnd);
/*		d_cvt_mat2pmat(n, n, B, n, 0, pE+n*bsd, pnd);*/
		
/*		d_print_pmat(n, 2*n, bsd, pE, 2*pnd);*/
/*		exit(2);*/
	
		for(i=0; i<pnd*cnd; i++) pC[i] = -1;
		
		for(i=0; i<pnd; i++) x[i] = 1;
		for(i=0; i<pnd; i++) x2[i] = 1;

		/* timing */
		struct timeval tvm1, tv0, tv1, tv2, tv3, tv4, tv5, tv6, tv7, tv8, tv9, tv10, tv11, tv12, tv13, tv14, tv15, tv16;

		/* warm up */
		for(rep=0; rep<nrep; rep++)
			{
			dgemm_nt_lib(n, n, n, pA, cnd, pB, cnd, pC, cnd, pC, cnd, 0, 0, 0);
			}

		gettimeofday(&tvm1, NULL); // start
	
		for(rep=0; rep<nrep; rep++)
			{

			dgemm_kernel_nt_lib(n, n, n, pA, cnd, pB, cnd, pC, cnd, pC, cnd, 0, 0, 0);

			}

		gettimeofday(&tv0, NULL); // start
	
		for(rep=0; rep<nrep; rep++)
			{

			dgemm_nt_lib(n, n, n, pA, cnd, pB, cnd, pC, cnd, pC, cnd, 0, 0, 0);

			}
	
		gettimeofday(&tv1, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{

			//dsyrk_dpotrf_lib(n, n, n, pE, cnd2, pD, cnd, diag, 1);
			dsyrk_dpotrf_lib(n, n, n, pA, cnd, pD, cnd, pC, cnd, diag, 1, 0);
			//d_print_pmat(pnd, cnd2, bsd, pE, cnd2);
			//exit(1);
			//break;

			}
	
		gettimeofday(&tv2, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{

			dtrmm_l_lib(n, n, pA, cnd, pB, cnd, pC, cnd);

			}
	
		gettimeofday(&tv3, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{

			dtrtr_l_lib(n, 0, pA, cnd, pC, cnd); // triangualr matrix transpose
			//dgetr_lib(n, n, 0, pA, cnd, 0, pC, cnd); // general matrix transpose

			}
	
		gettimeofday(&tv4, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{

			dgemv_n_lib(n, n, pA, cnd, x, y, y, 0);

			}
	
		gettimeofday(&tv5, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{

			dgemv_t_lib(n, n, pA, cnd, x, y, y, 0);

			}
	
		gettimeofday(&tv6, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{

			dtrmv_u_n_lib(n, pA, cnd, x, y, 0);

			}
	
		gettimeofday(&tv7, NULL); // stop


		for(rep=0; rep<nrep; rep++)
			{

			dtrmv_u_t_lib(n, pA, cnd, x, y, 0);

			}
	
		gettimeofday(&tv8, NULL); // stop


		for(rep=0; rep<nrep; rep++)
			{

			dtrsv_n_lib(2*n, n, 1, pF, cnd, x);

			}
	
		gettimeofday(&tv9, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{

			dtrsv_t_lib(2*n, n, 1, pF, cnd, x);

			}
	
		gettimeofday(&tv10, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{

			dsymv_lib(n, n, pA, cnd, x, y, y, 0);

			}
	
		gettimeofday(&tv11, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{

			dmvmv_lib(n, n, pA, cnd, x, y, y, x2, y2, y2, 0);

			}
	
		gettimeofday(&tv12, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{

			dsyrk_nt_lib(n, n, n, pE, cnd2, pE, cnd2, pD, cnd, pE+(n+pad)*bsd, cnd2, 1);
			dpotrf_lib(n, n, pE+(n+pad)*bsd, cnd2, pE+(n+pad)*bsd, cnd2, diag);
			//d_print_pmat(pnd, cnd2, bsd, pE, cnd2);
			//exit(1);
			//break;

			}
	
		gettimeofday(&tv13, NULL); // stop
	
		for(rep=0; rep<nrep; rep++)
			{
#if defined(REF_BLAS_OPENBLAS) || defined(REF_BLAS_NETLIB)
			dgemm_(&c_n, &c_n, &n, &n, &n, &d_1, A, &n, M, &n, &d_0, C, &n);
#endif
#if defined(REF_BLAS_BLIS)
			dgemm_(&c_n, &c_n, &n77, &n77, &n77, &d_1, A, &n77, B, &n77, &d_0, C, &n77);
#endif
			}

		gettimeofday(&tv14, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{
#if defined(REF_BLAS_OPENBLAS) || defined(REF_BLAS_NETLIB)
			dgemv_(&c_n, &n, &n, &d_1, A, &n, x2, &i_1, &d_0, y, &i_1);
#endif
#if defined(REF_BLAS_BLIS)
			dgemv_(&c_n, &n77, &n77, &d_1, A, &n77, x2, &i77_1, &d_0, y, &i77_1);
#endif
			}

		gettimeofday(&tv15, NULL); // stop

		for(rep=0; rep<nrep; rep++)
			{
#if defined(REF_BLAS_OPENBLAS) || defined(REF_BLAS_NETLIB)
			dgemv_(&c_t, &n, &n, &d_1, A, &n, x2, &i_1, &d_0, y, &i_1);
#endif
#if defined(REF_BLAS_BLIS)
			dgemv_(&c_t, &n77, &n77, &d_1, A, &n77, x2, &i77_1, &d_0, y, &i77_1);
#endif
			}

		gettimeofday(&tv16, NULL); // stop



		float Gflops_max = flops_max * GHz_max;

		float time_dgemm_kernel = (float) (tv0.tv_sec-tvm1.tv_sec)/(nrep+0.0)+(tv0.tv_usec-tvm1.tv_usec)/(nrep*1e6);
		float flop_dgemm_kernel = 2.0*n*n*n;
		float Gflops_dgemm_kernel = 1e-9*flop_dgemm_kernel/time_dgemm_kernel;

		float time_dgemm = (float) (tv1.tv_sec-tv0.tv_sec)/(nrep+0.0)+(tv1.tv_usec-tv0.tv_usec)/(nrep*1e6);
		float flop_dgemm = 2.0*n*n*n;
		float Gflops_dgemm = 1e-9*flop_dgemm/time_dgemm;

		float time_dsyrk_dpotrf = (float) (tv2.tv_sec-tv1.tv_sec)/(nrep+0.0)+(tv2.tv_usec-tv1.tv_usec)/(nrep*1e6);
		float flop_dsyrk_dpotrf = 1.0*n*n*n + 1.0/3.0*n*n*n;
		float Gflops_dsyrk_dpotrf = 1e-9*flop_dsyrk_dpotrf/time_dsyrk_dpotrf;

		float time_dtrmm = (float) (tv3.tv_sec-tv2.tv_sec)/(nrep+0.0)+(tv3.tv_usec-tv2.tv_usec)/(nrep*1e6);
		float flop_dtrmm = 1.0*n*n*n;
		float Gflops_dtrmm = 1e-9*flop_dtrmm/time_dtrmm;
	
		float time_dtrtr = (float) (tv4.tv_sec-tv3.tv_sec)/(nrep+0.0)+(tv4.tv_usec-tv3.tv_usec)/(nrep*1e6);
		float flop_dtrtr = 0.5*n*n;
		float Gflops_dtrtr = 1e-9*flop_dtrtr/time_dtrtr;

		float time_dgemv_n = (float) (tv5.tv_sec-tv4.tv_sec)/(nrep+0.0)+(tv5.tv_usec-tv4.tv_usec)/(nrep*1e6);
		float flop_dgemv_n = 2.0*n*n;
		float Gflops_dgemv_n = 1e-9*flop_dgemv_n/time_dgemv_n;

		float time_dgemv_t = (float) (tv6.tv_sec-tv5.tv_sec)/(nrep+0.0)+(tv6.tv_usec-tv5.tv_usec)/(nrep*1e6);
		float flop_dgemv_t = 2.0*n*n;
		float Gflops_dgemv_t = 1e-9*flop_dgemv_t/time_dgemv_t;

		float time_dtrmv_n = (float) (tv7.tv_sec-tv6.tv_sec)/(nrep+0.0)+(tv7.tv_usec-tv6.tv_usec)/(nrep*1e6);
		float flop_dtrmv_n = 1.0*n*n;
		float Gflops_dtrmv_n = 1e-9*flop_dtrmv_n/time_dtrmv_n;

		float time_dtrmv_t = (float) (tv8.tv_sec-tv7.tv_sec)/(nrep+0.0)+(tv8.tv_usec-tv7.tv_usec)/(nrep*1e6);
		float flop_dtrmv_t = 1.0*n*n;
		float Gflops_dtrmv_t = 1e-9*flop_dtrmv_t/time_dtrmv_t;

		float time_dtrsv_n = (float) (tv9.tv_sec-tv8.tv_sec)/(nrep+0.0)+(tv9.tv_usec-tv8.tv_usec)/(nrep*1e6);
		float flop_dtrsv_n = 3.0*n*n;
		float Gflops_dtrsv_n = 1e-9*flop_dtrsv_n/time_dtrsv_n;

		float time_dtrsv_t = (float) (tv10.tv_sec-tv9.tv_sec)/(nrep+0.0)+(tv10.tv_usec-tv9.tv_usec)/(nrep*1e6);
		float flop_dtrsv_t = 3.0*n*n;
		float Gflops_dtrsv_t = 1e-9*flop_dtrsv_t/time_dtrsv_t;

		float time_dsymv = (float) (tv11.tv_sec-tv10.tv_sec)/(nrep+0.0)+(tv11.tv_usec-tv10.tv_usec)/(nrep*1e6);
		float flop_dsymv = 2.0*n*n;
		float Gflops_dsymv = 1e-9*flop_dsymv/time_dsymv;

		float time_dmvmv = (float) (tv12.tv_sec-tv11.tv_sec)/(nrep+0.0)+(tv12.tv_usec-tv11.tv_usec)/(nrep*1e6);
		float flop_dmvmv = 4.0*n*n;
		float Gflops_dmvmv = 1e-9*flop_dmvmv/time_dmvmv;

		float time_dsyrk_dpotrf2 = (float) (tv13.tv_sec-tv12.tv_sec)/(nrep+0.0)+(tv13.tv_usec-tv12.tv_usec)/(nrep*1e6);
		float flop_dsyrk_dpotrf2 = 1.0*n*n*n + 1.0/3.0*n*n*n;
		float Gflops_dsyrk_dpotrf2 = 1e-9*flop_dsyrk_dpotrf2/time_dsyrk_dpotrf2;

		float time_dgemm_blas = (float) (tv14.tv_sec-tv13.tv_sec)/(nrep+0.0)+(tv14.tv_usec-tv13.tv_usec)/(nrep*1e6);
		float flop_dgemm_blas = 2.0*n*n*n;
		float Gflops_dgemm_blas = 1e-9*flop_dgemm_blas/time_dgemm_blas;

		float time_dgemv_n_blas = (float) (tv15.tv_sec-tv14.tv_sec)/(nrep+0.0)+(tv15.tv_usec-tv14.tv_usec)/(nrep*1e6);
		float flop_dgemv_n_blas = 2.0*n*n;
		float Gflops_dgemv_n_blas = 1e-9*flop_dgemv_n_blas/time_dgemv_n_blas;

		float time_dgemv_t_blas = (float) (tv16.tv_sec-tv15.tv_sec)/(nrep+0.0)+(tv16.tv_usec-tv15.tv_usec)/(nrep*1e6);
		float flop_dgemv_t_blas = 2.0*n*n;
		float Gflops_dgemv_t_blas = 1e-9*flop_dgemv_t_blas/time_dgemv_t_blas;

		printf("%d\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\n", n, Gflops_dgemm_kernel, 100.0*Gflops_dgemm_kernel/Gflops_max, Gflops_dgemm, 100.0*Gflops_dgemm/Gflops_max, Gflops_dsyrk_dpotrf, 100.0*Gflops_dsyrk_dpotrf/Gflops_max, Gflops_dtrmm, 100.0*Gflops_dtrmm/Gflops_max, Gflops_dtrtr, 100.0*Gflops_dtrtr/Gflops_max, Gflops_dgemv_n, 100.0*Gflops_dgemv_n/Gflops_max, Gflops_dgemv_t, 100.0*Gflops_dgemv_t/Gflops_max, Gflops_dtrmv_n, 100.0*Gflops_dtrmv_n/Gflops_max, Gflops_dtrmv_t, 100.0*Gflops_dtrmv_t/Gflops_max, Gflops_dtrsv_n, 100.0*Gflops_dtrsv_n/Gflops_max, Gflops_dtrsv_t, 100.0*Gflops_dtrsv_t/Gflops_max, Gflops_dsymv, 100.0*Gflops_dsymv/Gflops_max, Gflops_dmvmv, 100.0*Gflops_dmvmv/Gflops_max, Gflops_dsyrk_dpotrf2, 100.0*Gflops_dsyrk_dpotrf2/Gflops_max, Gflops_dgemm_blas, 100.0*Gflops_dgemm_blas/Gflops_max, Gflops_dgemv_n_blas, 100.0*Gflops_dgemv_n_blas/Gflops_max, Gflops_dgemv_t_blas, 100.0*Gflops_dgemv_t_blas/Gflops_max);

	fprintf(f, "%d\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\n", n, Gflops_dgemm_kernel, 100.0*Gflops_dgemm_kernel/Gflops_max, Gflops_dgemm, 100.0*Gflops_dgemm/Gflops_max, Gflops_dsyrk_dpotrf, 100.0*Gflops_dsyrk_dpotrf/Gflops_max, Gflops_dtrmm, 100.0*Gflops_dtrmm/Gflops_max, Gflops_dtrtr, 100.0*Gflops_dtrtr/Gflops_max, Gflops_dgemv_n, 100.0*Gflops_dgemv_n/Gflops_max, Gflops_dgemv_t, 100.0*Gflops_dgemv_t/Gflops_max, Gflops_dtrmv_n, 100.0*Gflops_dtrmv_n/Gflops_max, Gflops_dtrmv_t, 100.0*Gflops_dtrmv_t/Gflops_max, Gflops_dtrsv_n, 100.0*Gflops_dtrsv_n/Gflops_max, Gflops_dtrsv_t, 100.0*Gflops_dtrsv_t/Gflops_max, Gflops_dsymv, 100.0*Gflops_dsymv/Gflops_max, Gflops_dmvmv, 100.0*Gflops_dmvmv/Gflops_max, Gflops_dsyrk_dpotrf2, 100.0*Gflops_dsyrk_dpotrf2/Gflops_max, Gflops_dgemm_blas, 100.0*Gflops_dgemm_blas/Gflops_max, Gflops_dgemv_n_blas, 100.0*Gflops_dgemv_n_blas/Gflops_max, Gflops_dgemv_t_blas, 100.0*Gflops_dgemv_t_blas/Gflops_max);

		free(A);
		free(B);
		free(M);
		free(pA);
		free(pB);
		free(pC);
		free(pD);
		free(pE);
		free(pF);
		free(pL);
		free(pM);
		free(x);
		free(y);
		free(x2);
		free(y2);
		
		}

	printf("\n");

	fprintf(f, "];\n");
	fclose(f);

	return 0;
	
	}
