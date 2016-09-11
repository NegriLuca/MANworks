/* -*- c++ -*- (enableMbars emacs c++ mode) */
/*======================================================================
    "Mixed Finite Element Methods for Coupled 3D/1D Fluid Problems"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2016-2017
                  
                Copyright (C) 2016 Stefano Brambilla
======================================================================*/
/*! 
  @file   transport3d1d.cpp
  @author Stefano Brambilla <s.brambilla93@gmail.com>
  @date   September 2016.
  @brief  Definition of the main class for the 3D/1D coupled transport problem.
 */
 
 #include <transport3d1d.hpp>
 
 namespace getfem {
 
 void transport3d1d::init (int argc, char *argv[])
 {
 std::cout << "initialize transport problem..."<<std::endl<<std::endl;
 
 import_data();
 set_im_and_fem();
 build_param();
 
 }; // end of init
 
 void transport3d1d::assembly (void)
 {
 std::cout<<"assemble transport problem"<<std::endl<<std::endl;
 
 }; // end of assembly
 

 bool transport3d1d::solve (void)
 {
  std::cout<<"solve transport problem"<<std::endl<<std::endl;
 }; // end of solve
	
	
 void transport3d1d::export_vtk (const string & suff)
 {
  std::cout<<"export transport problem"<<std::endl<<std::endl;
 }; // end of export
  
  
  
  

 // Aux methods for init
	
	//! Import algorithm specifications
	void transport3d1d::import_data(void)
	{
		std::cout<<"init part 1: import data!......" <<std::endl;
	#ifdef M3D1D_VERBOSE_
	cout << "Importing descriptors for tissue and vessel problems ..." << endl;
	#endif
	descr_transp.import_transp(PARAM);
	#ifdef M3D1D_VERBOSE_
	cout << descr_transp;
	#endif
	

	};
	
	
	
	//! Import mesh for tissue (3D) and vessel (1D)  
	//void build_mesh(void); 
	//! Set finite elements methods and integration methods 
	void transport3d1d::set_im_and_fem(void)
	{
	std::cout<<"init part 2: set fem methods!......" <<std::endl;
	

	#ifdef M3D1D_VERBOSE_
	cout << "Setting FEMs for tissue and vessel problems ..." << endl;
	#endif
	

		
	pfem pf_Ct = fem_descriptor(descr_transp.FEM_TYPET_C);
	pfem pf_Cv = fem_descriptor(descr_transp.FEM_TYPEV_C);

	#ifdef M3D1D_VERBOSE_
	cout << "Setting IMs and FEMs for tissue ..." << endl;
	#endif
		

	mf_Ct.set_finite_element(mesht.convex_index(), pf_Ct);

	#ifdef M3D1D_VERBOSE_
	cout << "Setting IMs and FEMs for vessel branches ..." << endl;
	#endif

	mf_Cv.set_finite_element(meshv.convex_index(), pf_Cv);

	
	#ifdef M3D1D_VERBOSE_
	cout << "Setting FEM dimensions for tissue and vessel problems ..." << endl;
	#endif

	dof_transp.set(mf_Ct, mf_Cv);
	#ifdef M3D1D_VERBOSE_
	cout << std::scientific << dof_transp;
	#endif
	

	};
	
	
	//! Build problem parameters
	void transport3d1d::build_param(void)
	{
	std::cout<<"init part 3: build dimensionless parameters!" <<std::endl;
	
	#ifdef M3D1D_VERBOSE_
	cout << "Building parameters for tissue and vessel problems ..." << endl;
	#endif
	param_transp.build(PARAM, mf_coeft, mf_coefv);
	#ifdef M3D1D_VERBOSE_
	cout << param_transp ;
	#endif
	cout<<param_transp;
	};
  
  
  
  
  
  
 
 } // end of namespace
