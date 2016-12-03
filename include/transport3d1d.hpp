/* -*- c++ -*- (enableMbars emacs c++ mode) */
/*======================================================================
    "Mixed Finite Element Methods for Coupled 3D/1D Fluid Problems"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2016-2017
                  
                Copyright (C) 2016 Stefano Brambilla
======================================================================*/
/*! 
  @file   transport3d1d.hpp
  @author Stefano Brambilla <s.brambilla93@gmail.com>
  @date   September 2016.
  @brief  Declaration of the main class for the 3D/1D coupled transport problem.
 */
 
#ifndef M3D1D_TRANSPORT3D1D_HPP_
#define M3D1D_TRANSPORT3D1D_HPP_
#define M3D1D_VERBOSE_

// GetFem++ libraries
#include <getfem/getfem_assembling.h> 
#include <getfem/getfem_import.h>
#include <getfem/getfem_export.h>   
#include <getfem/getfem_regular_meshes.h>
#include <getfem/getfem_mesh.h>
#include <getfem/getfem_derivatives.h>
#include <getfem/getfem_superlu.h>
#include <getfem/getfem_partial_mesh_fem.h>
#include <getfem/getfem_interpolated_fem.h>
#include <gmm/gmm.h>
#include <gmm/gmm_inoutput.h>
#include <gmm/gmm_iter_solvers.h>
// Standard libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>
// Project headers
#include <defines.hpp>
#include <mesh3d.hpp>       
#include <mesh1d.hpp>
#include <utilities.hpp>

#include <assembling1d.hpp>          
#include <assembling3d.hpp>        
#include <assembling3d1d.hpp>
#include <node.hpp>
#include <dof3d1d.hpp>
#include <descr3d1d.hpp>
#include <param3d1d.hpp>

#include <assembling1d_transp.hpp>          
#include <assembling3d_transp.hpp>        
#include <assembling3d1d_transp.hpp>
#include <dof3d1d_transp.hpp>
#include <descr3d1d_transp.hpp>
#include <param3d1d_transp.hpp>


//#include <defines.hpp>>
#include <problem3d1d.hpp>

 
 namespace getfem {

//!	Main class defining the coupled 3D/1D transport problem.
class transport3d1d: public problem3d1d {

public:
	transport3d1d(void) : 
		/*problem3d1d.mimt(mesht),  problem3d1d::mimv(meshv),
		problem3d1d::mf_Pt(mesht), problem3d1d::mf_coeft(mesht), problem3d1d::mf_Ut(mesht),
		problem3d1d::mf_Pv(meshv), problem3d1d::mf_coefv(meshv),*/
		mf_Ct(mesht), mf_Cv(meshv)
	{} 
	//! Initialize the problem
	void init (int argc, char *argv[]);
	
	//! Assemble the problem
	void assembly (void);

	//! Solve the problem
	bool solve (void);
	
	//! Export the solution
	void export_vtk (const string & suff = ""); //const string & time_suff = "", 

protected:
	 
	//! Mesh for the vessel network for transport problem @f$\Lambda_(transp)@f$ (1D)
	mesh meshv_transp;
	
	//! Finite Element Method for the tissue pressure @f$p_v@f$
	mesh_fem mf_Ct; 
	//! Finite Element Method for the vessel pressure @f$p_v@f$
	mesh_fem mf_Cv; 
	
	//! Algorithm description strings (mesh files, FEM types, solver info, ...) 
	descr3d1d_transp descr_transp;
	//! Physical parameters (dimensionless)
	param3d1d_transp param_transp;
	//! Number of degrees of freedom
	dof3d1d_transp dof_transp;
		

	
////	//eventualmente i fem dei coefficienti Lp_lf e m se variano dentro la mesh
	
	//! List of BC nodes of the network
	vector< node > BCv_transp;	
		
	//! Monolithic matrix for the coupled problem
	sparse_matrix_type AM_transp;
	//! Monolithic array of unknowns for the coupled problem
	vector_type        UM_transp;
	//! Monolithic right hand side for the coupled problem
	vector_type        FM_transp;
	
	
////	//T totale, deltaT, T iniziale, T stop
	//matrice con le soluzioni della concentrazione ad ogni t per ogni colonna
	

	// Aux methods for init
	//! Import algorithm specifications
	void import_data(void);
	//! Import mesh for tissue (3D) and vessel (1D)  
	void build_mesh(void); 
	//! Set finite elements methods and integration methods 
	void set_im_and_fem(void);
	//! Build problem parameters
	void build_param(void);
	//! Build the monolithic matrix AM_transp by blocks
	void assembly_mat(void);
	//! Build the monolithic rhs FM_transp by blocks
	void assembly_rhs(void);
	
	
	
}; //end of class trasport3d1d

}  //end of namespace

#endif
