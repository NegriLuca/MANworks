 /* -*- c++ -*- (enableMbars emacs c++ mode) */
/*======================================================================
    "Mixed Finite Element Methods for Coupled 3D/1D Fluid Problems"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2016-2017
                  
                Copyright (C) 2016 Stefano Brambilla
======================================================================*/
/*! 
  @file   assembling3d_transp.hpp
  @author Stefano Brambilla <s.brambilla93@gmail.com>
  @date   September 2016.
  @brief  Miscelleanous assembly routines for the 3D transport problem.
 */
#ifndef M3D1D_ASSEMBLING_3D_TRANSP_HPP_
#define M3D1D_ASSEMBLING_3D_TRANSP_HPP_
#include <defines.hpp>
#include <utilities.hpp>

namespace getfem {

//! Build the mass, reaction and diffusion matrices for the 3D transport problem,
//! @f$ M = \int_{\Omega} u~v~dx @f$ and
//! @f$ R = \int_{\Omega} r~u~v~dx @f$ and
//! @f$ D = \int_{\Omega} }  \nabla u \nabla \cdotv~dx @f$
/*! 
	@param M            Mass matrix
	@param D            Diffusion matrix
	@param R            Reaction matrix
	@param mim          The integration method to be used
	@param mf_c         The finite element method for the concentration @f$ c @f$
	@param reac_data    Parameter for reaction term
	@param diff_data    Parameter for diffusion term
	@param rg           The region where to integrate

	@ingroup asm
 */ 
template<typename MAT, typename VEC>
void 
asm_tissue_transp
	(MAT & M, MAT & D,MAT & R,
	 const mesh_im & mim,
	 const mesh_fem & mf_c,
	 const mesh_fem & mf_coef,
	 const VEC & reac_data,
	 const VEC & diff_data,
	 const mesh_region & rg = mesh_region::all_convexes()
	 ) 		
{
	GMM_ASSERT1(mf_c.get_qdim() == 1, 
		"invalid data mesh fem for pressure (Qdim=1 required)");
	// Build the mass matrix Mt (consumption)
	getfem::asm_mass_matrix_param(R, mim, mf_c, mf_coef, reac_data, rg);
	// Build the mass matrix Tt for time derivative 
	getfem::asm_mass_matrix(M, mim, mf_c, rg);
	// Build the divergence matrix Dtt
	getfem::asm_stiffness_matrix_for_laplacian(D,mim,mf_c, mf_coef, diff_data, rg); 
	
} /* end of asm_tissue_darcy*/

//! Build the advection matrice for the 3D transport problem,
//! @f$ B = \int_{\Omega} \mathbf{U} \cdot \nabla u~v~dx @f$
/*! 
	@param B            Advection matrix
	@param mim          The integration method to be used
	@param mf           The finite element method for the concentration @f$ c @f$
	@param mf vel       The finite element method for the velocity field @f$ U @f$
	@param vel          The velocity field
	@param rg           The region where to integrate

	@ingroup asm
 */  
  template<typename MAT, typename VECT>
  void asm_advection_tissue(MAT &B, const getfem::mesh_im &mim,
			    const getfem::mesh_fem &mf,
                            const getfem::mesh_fem &mfvel,
                            const VECT &vel,
                            	 const mesh_region & rg = mesh_region::all_convexes()                            	 
                            	 ) {
    getfem::generic_assembly
      assem("vel=data(#2);"
            "M$1(#1,#1) += comp(Base(#1).Grad(#1).vBase(#2))"
            "(:, :,i, k,i).vel(k);");
    assem.push_mi(mim);
    assem.push_mf(mf);
    assem.push_mf(mfvel);
    assem.push_data(vel);
    assem.push_mat(B);
    assem.assembly(rg);
  }  /* end of asm_advection_matrix*/


/*! Build the mixed boundary conditions (weak form) and dirichlet (strong form) for vessels
    @f$ M=\int_{\Gamma_{MIX}} \beta~u~v~d\sigma@f$ and
    @f$ F=\int_{\Gamma_{MIX}} \beta~c0~v~d\sigma@f$
 */
/*!
	@param F        BC contribution to rhs
	@param M        BC contribution to mass matrix
	@param mim      The integration method to be used
	@param mf_c     The finite element method for the concentration @f$u@f$
	@param mf_data  The finite element method for the coefficients
	@param BC       Array of values of network boundary points
	@param beta     The beta value for mix condition @f$p_0@f$
	@ingroup asm
 */ 
template<typename MAT, typename VEC>
void
asm_tissue_bc_transp
	(VEC & F,
	 MAT & M,
	 const mesh_im & mim,
	 const mesh_fem & mf_c,
	 const mesh_fem & mf_data,
	 const std::vector<getfem::node> & BC,
	 const scalar_type beta
	 )
{

	
	
	GMM_ASSERT1(mf_c.get_qdim()==1,  "invalid data mesh fem (Qdim=1 required)");
	GMM_ASSERT1(mf_data.get_qdim()==1, "invalid data mesh fem (Qdim=1 required)");


	for (size_type bc=0; bc < BC.size(); ++bc) {
		GMM_ASSERT1(mf_c.linked_mesh().has_region(bc), "missed mesh region" << bc);
		if (BC[bc].label=="DIR") { // Dirichlet BC
			VEC BC_temp(mf_data.nb_dof(), BC[bc].value);
			getfem::assembling_Dirichlet_condition(M, F, mf_c, BC[bc].rg, BC_temp);
			gmm::clear(BC_temp);				
		} 
		else if (BC[bc].label=="MIX") { // Robin BC
			VEC BETA(mf_data.nb_dof(), beta);
			getfem::asm_mass_matrix_param(M, mim, mf_c, mf_data, BETA,mf_c.linked_mesh().region(BC[bc].rg) );
			
			VEC BETA_C0(mf_data.nb_dof(), beta*BC[bc].value);
			asm_source_term(F,mim, mf_c, mf_data,BETA_C0);
			
		}
		else if (BC[bc].label=="INT") { // Internal Node
			DAL_WARNING1("internal node passed as boundary.");
		}
		else if (BC[bc].label=="JUN") { // Junction Node
			DAL_WARNING1("junction node passed as boundary.");
		}
		else {
			GMM_ASSERT1(0, "Unknown Boundary Condition " << BC[bc].label << endl);
		}
	}

} /* end of asm_tissue_bc */



} /* end of namespace */

#endif
