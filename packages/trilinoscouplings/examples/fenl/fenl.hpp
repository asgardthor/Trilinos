/*
//@HEADER
// ************************************************************************
//
//   Kokkos: Manycore Performance-Portable Multidimensional Arrays
//              Copyright (2012) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact  H. Carter Edwards (hcedwar@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#ifndef KOKKOS_EXAMPLE_FENL_HPP
#define KOKKOS_EXAMPLE_FENL_HPP

#include <iostream>

#include <stdlib.h>
#include <BoxElemPart.hpp>

#include <Teuchos_Comm.hpp>
#include <Teuchos_GlobalMPISession.hpp>
#include <KokkosCompat_ClassicNodeAPI_Wrapper.hpp>

#include <Kokkos_View.hpp>
#include <Kokkos_ArithTraits.hpp>

namespace Kokkos {
namespace Example {
namespace FENL {

struct Perf {
  size_t uq_count ;
  size_t global_elem_count ;
  size_t global_node_count ;
  size_t newton_iter_count ;
  size_t cg_iter_count ;
  double map_ratio ;
  double fill_node_set ;
  double scan_node_count ;
  double fill_graph_entries ;
  double sort_graph_entries ;
  double fill_element_graph ;
  double create_sparse_matrix ;
  double fill_time ;
  double bc_time ;
  double cg_time ;
  double newton_residual ;
  double error_max ;
  double response_mean ;
  double response_std_dev ;

  Perf() : uq_count(1) ,
           global_elem_count(0) ,
           global_node_count(0) ,
           newton_iter_count(0) ,
           cg_iter_count(0) ,
           map_ratio(0) ,
           fill_node_set(0) ,
           scan_node_count(0) ,
           fill_graph_entries(0) ,
           sort_graph_entries(0) ,
           fill_element_graph(0) ,
           create_sparse_matrix(0) ,
           fill_time(0) ,
           bc_time(0) ,
           cg_time(0) ,
           newton_residual(0) ,
           error_max(0) ,
           response_mean(0) ,
           response_std_dev(0) {}

  void increment(const Perf& p) {
    global_elem_count     = p.global_elem_count;
    global_node_count     = p.global_node_count;

    newton_iter_count    += p.newton_iter_count;
    cg_iter_count        += p.cg_iter_count;
    map_ratio            += p.map_ratio;
    fill_node_set        += p.fill_node_set;
    scan_node_count      += p.scan_node_count;
    fill_graph_entries   += p.fill_graph_entries;
    sort_graph_entries   += p.sort_graph_entries;
    fill_element_graph   += p.fill_element_graph;
    create_sparse_matrix += p.create_sparse_matrix;
    fill_time            += p.fill_time;
    bc_time              += p.bc_time ;
    cg_time              += p.cg_time;
    newton_residual      += p.newton_residual ;
    error_max            += p.error_max;
  }
};

//----------------------------------------------------------------------------

template < class Scalar, class Device , BoxElemPart::ElemOrder ElemOrder,
           class CoeffFunctionType , class ManufacturedSolutionType >
Perf fenl(
  const Teuchos::RCP<const Teuchos::Comm<int> >& comm ,
  const Teuchos::RCP< Kokkos::Compat::KokkosDeviceWrapperNode<Device> >& node,
  const int use_print ,
  const int use_trials ,
  const int use_atomic ,
  const int use_belos ,
  const int use_muelu ,
  const int use_nodes[] ,
  const CoeffFunctionType& coeff_function ,
  const ManufacturedSolutionType& manufactured_solution ,
  const double bc_lower_value ,
  const double bc_upper_value ,
  const bool check_solution ,
  Scalar& response );

//----------------------------------------------------------------------------
// Manufactured Solutions
//----------------------------------------------------------------------------

class ManufacturedSolution {
public:

  // Manufactured solution for one dimensional nonlinear PDE
  //
  //  -K T_zz + T^2 = 0 ; T(zmin) = T_zmin ; T(zmax) = T_zmax
  //
  //  Has an analytic solution of the form:
  //
  //    T(z) = ( a ( z - zmin ) + b )^(-2) where K = 1 / ( 6 a^2 )
  //
  //  Given T_0 and T_L compute K for this analytic solution.
  //
  //  Two analytic solutions:
  //
  //    Solution with singularity:
  //    , a( ( 1.0 / sqrt(T_zmax) + 1.0 / sqrt(T_zmin) ) / ( zmax - zmin ) )
  //    , b( -1.0 / sqrt(T_zmin) )
  //
  //    Solution without singularity:
  //    , a( ( 1.0 / sqrt(T_zmax) - 1.0 / sqrt(T_zmin) ) / ( zmax - zmin ) )
  //    , b( 1.0 / sqrt(T_zmin) )

  const double zmin ;
  const double zmax ;
  const double T_zmin ;
  const double T_zmax ;
  const double a ;
  const double b ;
  const double K ;

  ManufacturedSolution( const double arg_zmin ,
                        const double arg_zmax ,
                        const double arg_T_zmin ,
                        const double arg_T_zmax )
    : zmin( arg_zmin )
    , zmax( arg_zmax )
    , T_zmin( arg_T_zmin )
    , T_zmax( arg_T_zmax )
    , a( ( 1.0 / sqrt(T_zmax) - 1.0 / sqrt(T_zmin) ) / ( zmax - zmin ) )
    , b( 1.0 / sqrt(T_zmin) )
    , K( 1.0 / ( 6.0 * a * a ) )
    {}

  double operator()( const double z ) const
  {
    const double tmp = a * ( z - zmin ) + b ;
    return 1.0 / ( tmp * tmp );
  }

  template <typename FixtureType>
  void print( std::ostream& os , const FixtureType& fixture ) const {
    os << "Manufactured solution"
       << " a[" << a << "]"
       << " b[" << b << "]"
       << " K[" << K << "]"
       << " {" ;
    for ( unsigned inode = 0 ; inode < fixture.node_count() ; ++inode ) {
      os << " " << (*this)( fixture.node_coord( inode , 2 ) );
    }
    os << " }" << std::endl ;
  }

  template < typename FixtureType , typename SolutionType >
  double compute_error( const FixtureType& fixture ,
                        const SolutionType& solution ) const {
    typedef typename SolutionType::value_type Scalar;
    typedef Kokkos::Details::ArithTraits<Scalar> KAT;
    typedef typename KAT::mag_type Magnitude;

    const typename FixtureType::node_coord_type::HostMirror
      h_node_coord = Kokkos::create_mirror_view( fixture.node_coord() );

    const typename SolutionType::HostMirror
      h_nodal_solution = Kokkos::create_mirror_view( solution );

    Kokkos::deep_copy( h_node_coord , fixture.node_coord() );
    Kokkos::deep_copy( h_nodal_solution , solution );

    double error_max = 0 ;
    for ( unsigned inode = 0 ; inode < fixture.node_count_owned() ; ++inode ) {
      const double answer = (*this)( h_node_coord( inode , 2 ) );
      const Magnitude error =
        KAT::abs( ( h_nodal_solution(inode) - answer ) / answer );
      if ( error_max < error ) { error_max = error ; }
    }

    return  error_max;
  }
};

//----------------------------------------------------------------------------

class TrivialManufacturedSolution {
public:

  // A trivial manufactured solution that doesn't do anything

  TrivialManufacturedSolution() {}

  template <typename FixtureType>
  void print( std::ostream& os , const FixtureType& fixture ) const {}

  template < typename FixtureType , typename SolutionType >
  double compute_error( const FixtureType& fixture ,
                        const SolutionType& solution ) const {
    return 0.0;
  }
};

//----------------------------------------------------------------------------
// Diffusion coefficients
//----------------------------------------------------------------------------

struct ElementComputationConstantCoefficient {
  enum { is_constant = true };

  const float coeff_k ;

  KOKKOS_INLINE_FUNCTION
  float operator()( double /* x */
                  , double /* y */
                  , double /* z */
                  ) const
    { return coeff_k ; }

  ElementComputationConstantCoefficient( const float val )
    : coeff_k( val ) {}

  ElementComputationConstantCoefficient( const ElementComputationConstantCoefficient & rhs )
    : coeff_k( rhs.coeff_k ) {}
};

//----------------------------------------------------------------------------

template < typename Scalar, typename MeshScalar, typename Device >
class ElementComputationKLCoefficient {
public:

  enum { is_constant = false };
  typedef Kokkos::View<Scalar*, Device>          RandomVariableView;
  typedef Kokkos::View<MeshScalar*, Device>      EigenView;
  typedef typename RandomVariableView::size_type size_type;

private:

  const MeshScalar m_mean;        // Mean of random field
  const MeshScalar m_variance;    // Variance of random field
  const MeshScalar m_corr_len;    // Correlation length of random field
  const size_type m_num_rv;       // Number of random variables
  RandomVariableView m_rv;        // KL random variables
  const EigenView m_eig;          // KL eigenvalues
  const MeshScalar m_pi;          // 3.1415...

public:

  ElementComputationKLCoefficient( const MeshScalar mean ,
                                   const MeshScalar variance ,
                                   const MeshScalar correlation_length ,
                                   const size_type num_rv );

  ElementComputationKLCoefficient( const ElementComputationKLCoefficient & rhs )
    : m_mean( rhs.m_mean ) ,
      m_variance( rhs.m_variance ) ,
      m_corr_len( rhs.m_corr_len ) ,
      m_num_rv( rhs.m_num_rv ) ,
      m_rv( rhs.m_rv ) ,
      m_eig( rhs.m_eig ) ,
      m_pi( rhs.m_pi ) {}

  KOKKOS_INLINE_FUNCTION
  void setRandomVariables( const RandomVariableView& rv) { m_rv = rv; }

  KOKKOS_INLINE_FUNCTION
  RandomVariableView getRandomVariables() { return m_rv; }

  KOKKOS_INLINE_FUNCTION
  Scalar operator() ( const MeshScalar x,
                      const MeshScalar y,
                      const MeshScalar z ) const
  {
    Scalar val = 0.0;
    if (m_num_rv > 0)
      val += m_eig(0) * m_rv(0);
    for ( size_type i=1; i<m_num_rv; i+=2 ) {
      const MeshScalar b = (i+1)/2;  // floor((i+1)/2)
      val += m_eig(i) * std::sin( b * m_pi * x ) * m_rv(i);
    }
    for ( size_type i=2; i<m_num_rv; i+=2 ) {
      const MeshScalar b = (i+1)/2;  // floor((i+1)/2)
      val += m_eig(i) * std::cos( b * m_pi * x ) * m_rv(i);
    }

    val = m_mean + m_variance * val;

    return val;
  }
};

} /* namespace FENL */
} /* namespace Example */
} /* namespace Kokkos */

#endif /* #ifndef KOKKOS_EXAMPLE_FENL_HPP */
