/*
 boost header: boost_numeric_odeint/explicit_error_dopri5.hpp

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_BOOST_NUMERIC_ODEINT_STEPPER_EXPLICIT_ERROR_DOPRI5_HPP_INCLUDED
#define BOOST_BOOST_NUMERIC_ODEINT_STEPPER_EXPLICIT_ERROR_DOPRI5_HPP_INCLUDED

#include <boost/ref.hpp>

#include <boost/numeric/odeint/stepper/base/explicit_stepper_and_error_stepper_fsal_base.hpp>
#include <boost/numeric/odeint/algebra/range_algebra.hpp>
#include <boost/numeric/odeint/algebra/default_operations.hpp>
#include <boost/numeric/odeint/stepper/stepper_categories.hpp>
#include <boost/numeric/odeint/stepper/detail/macros.hpp>

namespace boost {
namespace numeric {
namespace odeint {

/*
 * ToDo: Check orders rk_ckc
 */
template<
    class State ,
    class Value = double ,
    class Deriv = State ,
    class Time = double ,
	class Algebra = range_algebra ,
	class Operations = default_operations ,
	class AdjustSizePolicy = adjust_size_initially_tag
	>
class explicit_error_dopri5
: public explicit_stepper_and_error_stepper_fsal_base<
	  explicit_error_dopri5< State , Value , Deriv , Time , Algebra , Operations , AdjustSizePolicy > ,
	  5 , 5 , 4 , State , Value , Deriv , Time , Algebra , Operations , AdjustSizePolicy >
{

	void initialize( void )
	{
		boost::numeric::odeint::construct( m_x_tmp );
		boost::numeric::odeint::construct( m_k2 );
		boost::numeric::odeint::construct( m_k3 );
		boost::numeric::odeint::construct( m_k4 );
		boost::numeric::odeint::construct( m_k5 );
		boost::numeric::odeint::construct( m_k6 );
		m_state_adjuster.register_state( 0 , m_x_tmp );
		m_deriv_adjuster.register_state( 0 , m_k2 );
		m_deriv_adjuster.register_state( 1 , m_k3 );
		m_deriv_adjuster.register_state( 2 , m_k4 );
		m_deriv_adjuster.register_state( 3 , m_k5 );
		m_deriv_adjuster.register_state( 4 , m_k6 );
	}

	void copy( const explicit_error_dopri5 &d )
	{
		boost::numeric::odeint::copy( d.m_x_tmp , m_x_tmp );
		boost::numeric::odeint::copy( d.m_k2 , m_k2 );
		boost::numeric::odeint::copy( d.m_k3 , m_k3 );
		boost::numeric::odeint::copy( d.m_k4 , m_k4 );
		boost::numeric::odeint::copy( d.m_k5 , m_k5 );
		boost::numeric::odeint::copy( d.m_k6 , m_k6 );
	}



public :

	BOOST_ODEINT_EXPLICIT_STEPPERS_AND_ERROR_STEPPERS_FSAL_TYPEDEFS( explicit_error_dopri5 , 5 , 5 , 4 );


	explicit_error_dopri5( void )
	: stepper_base_type() , m_state_adjuster() , m_deriv_adjuster() , m_x_tmp() , m_k2() , m_k3() , m_k4() , m_k5() , m_k6()
	{
		initialize();
	}

	~explicit_error_dopri5( void )
	{
		boost::numeric::odeint::destruct( m_x_tmp );
		boost::numeric::odeint::destruct( m_k2 );
		boost::numeric::odeint::destruct( m_k3 );
		boost::numeric::odeint::destruct( m_k4 );
		boost::numeric::odeint::destruct( m_k5 );
		boost::numeric::odeint::destruct( m_k6 );
	}

	explicit_error_dopri5( const explicit_error_dopri5 &d )
	: stepper_base_type( d ) , m_state_adjuster() , m_deriv_adjuster() , m_x_tmp() , m_k2() , m_k3() , m_k4() , m_k5() , m_k6()
	{
		initialize();
		copy( d );
	}

	explicit_error_dopri5& operator=( const explicit_error_dopri5 &d )
	{
		stepper_base_type::operator=( d );
		copy( d );
		return *this;
	}






	template< class System , class StateIn , class DerivIn , class StateOut , class DerivOut >
	void do_step_impl( System system , const StateIn &in , const DerivIn &dxdt_in , const time_type &t ,
			                           StateOut &out , DerivOut &dxdt_out , const time_type &dt )
	{
	    const value_type a2 = static_cast<value_type> ( 0.2 );
        const value_type a3 = static_cast<value_type> ( 0.3 );
        const value_type a4 = static_cast<value_type> ( 0.8 );
        const value_type a5 = static_cast<value_type> ( 8.0 )/static_cast<value_type> ( 9.0 );

        const value_type b21 = static_cast<value_type> ( 0.2 );

        const value_type b31 = static_cast<value_type> ( 3.0 ) / static_cast<value_type>( 40.0 );
        const value_type b32 = static_cast<value_type> ( 9.0 ) / static_cast<value_type>( 40.0 );

        const value_type b41 = static_cast<value_type> ( 44.0 ) / static_cast<value_type> ( 45.0 );
        const value_type b42 = static_cast<value_type> ( -56.0 ) / static_cast<value_type> ( 15.0 );
        const value_type b43 = static_cast<value_type> ( 32.0 ) / static_cast<value_type> ( 9.0 );

        const value_type b51 = static_cast<value_type> ( 19372.0 ) / static_cast<value_type>( 6561.0 );
        const value_type b52 = static_cast<value_type> ( -25360.0 ) / static_cast<value_type> ( 2187.0 );
        const value_type b53 = static_cast<value_type> ( 64448.0 ) / static_cast<value_type>( 6561.0 );
        const value_type b54 = static_cast<value_type> ( -212.0 ) / static_cast<value_type>( 729.0 );

        const value_type b61 = static_cast<value_type> ( 9017.0 ) / static_cast<value_type>( 3168.0 );
        const value_type b62 = static_cast<value_type> ( -355.0 ) / static_cast<value_type>( 33.0 );
        const value_type b63 = static_cast<value_type> ( 46732.0 ) / static_cast<value_type>( 5247.0 );
        const value_type b64 = static_cast<value_type> ( 49.0 ) / static_cast<value_type>( 176.0 );
        const value_type b65 = static_cast<value_type> ( -5103.0 ) / static_cast<value_type>( 18656.0 );

        const value_type c1 = static_cast<value_type> ( 35.0 ) / static_cast<value_type>( 384.0 );
        const value_type c3 = static_cast<value_type> ( 500.0 ) / static_cast<value_type>( 1113.0 );
        const value_type c4 = static_cast<value_type> ( 125.0 ) / static_cast<value_type>( 192.0 );
        const value_type c5 = static_cast<value_type> ( -2187.0 ) / static_cast<value_type>( 6784.0 );
        const value_type c6 = static_cast<value_type> ( 11.0 ) / static_cast<value_type>( 84.0 );

	    m_state_adjuster.adjust_size_by_policy( in , adjust_size_policy() );
	    m_deriv_adjuster.adjust_size_by_policy( in , adjust_size_policy() );

		typename boost::unwrap_reference< System >::type &sys = system;

        //m_x_tmp = x + dt*b21*dxdt
        typename algebra_type::for_each3()( m_x_tmp , in , dxdt_in ,
        		typename operations_type::template scale_sum2< value_type , time_type >( 1.0 , dt*b21 ) );

        sys( m_x_tmp , m_k2 , t + dt*a2 );
        // m_x_tmp = x + dt*b31*dxdt + dt*b32*m_k2
        typename algebra_type::for_each4()( m_x_tmp , in , dxdt_in , m_k2 ,
        		typename operations_type::template scale_sum3< value_type , time_type , time_type >( 1.0 , dt*b31 , dt*b32 ));

        sys( m_x_tmp , m_k3 , t + dt*a3 );
        // m_x_tmp = x + dt * (b41*dxdt + b42*m_k2 + b43*m_k3)
        typename algebra_type::for_each5()( m_x_tmp , in , dxdt_in , m_k2 , m_k3 ,
        		typename operations_type::template scale_sum4< value_type , time_type , time_type , time_type >( 1.0 , dt*b41 , dt*b42 , dt*b43 ));

        sys( m_x_tmp, m_k4 , t + dt*a4 );
        typename algebra_type::for_each6()( m_x_tmp , in , dxdt_in , m_k2 , m_k3 , m_k4 ,
        		typename operations_type::template scale_sum5< value_type , time_type , time_type , time_type , time_type >( 1.0 , dt*b51 , dt*b52 , dt*b53 , dt*b54 ));

        sys( m_x_tmp , m_k5 , t + dt*a5 );
        typename algebra_type::for_each7()( m_x_tmp , in , dxdt_in , m_k2 , m_k3 , m_k4 , m_k5 ,
        		typename operations_type::template scale_sum6< value_type , time_type , time_type , time_type , time_type , time_type >( 1.0 , dt*b61 , dt*b62 , dt*b63 , dt*b64 , dt*b65 ));

        sys( m_x_tmp , m_k6 , t + dt );
        typename algebra_type::for_each7()( out , in , dxdt_in , m_k3 , m_k4 , m_k5 , m_k6 ,
        		typename operations_type::template scale_sum6< value_type , time_type , time_type , time_type , time_type , time_type >( 1.0 , dt*c1 , dt*c3 , dt*c4 , dt*c5 , dt*c6 ));

        // the new derivative
        sys( out , dxdt_out , t + dt );
	}


	template< class System , class StateIn , class DerivIn , class StateOut , class DerivOut , class Err >
	void do_step_impl( System system , const StateIn &in , const DerivIn &dxdt_in , const time_type &t ,
			                           StateOut &out , DerivOut &dxdt_out , const time_type &dt , Err &xerr )
	{
        const value_type c1 = static_cast<value_type> ( 35.0 ) / static_cast<value_type>( 384.0 );
        const value_type c3 = static_cast<value_type> ( 500.0 ) / static_cast<value_type>( 1113.0 );
        const value_type c4 = static_cast<value_type> ( 125.0 ) / static_cast<value_type>( 192.0 );
        const value_type c5 = static_cast<value_type> ( -2187.0 ) / static_cast<value_type>( 6784.0 );
        const value_type c6 = static_cast<value_type> ( 11.0 ) / static_cast<value_type>( 84.0 );

        const value_type dc1 = c1 - static_cast<value_type> ( 5179.0 ) / static_cast<value_type>( 57600.0 );
        const value_type dc3 = c3 - static_cast<value_type> ( 7571.0 ) / static_cast<value_type>( 16695.0 );
        const value_type dc4 = c4 - static_cast<value_type> ( 393.0 ) / static_cast<value_type>( 640.0 );
        const value_type dc5 = c5 - static_cast<value_type> ( -92097.0 ) / static_cast<value_type>( 339200.0 );
        const value_type dc6 = c6 - static_cast<value_type> ( 187.0 ) / static_cast<value_type>( 2100.0 );
        const value_type dc7 = static_cast<value_type>( -0.025 );

        do_step_impl( system , in , dxdt_in , t , out , dxdt_out , dt );

        //error estimate
        typename algebra_type::for_each7()( xerr , dxdt_in , m_k3 , m_k4 , m_k5 , m_k6 , dxdt_out ,
                    typename operations_type::template scale_sum6< time_type , time_type , time_type , time_type , time_type , time_type >( dt*dc1 , dt*dc3 , dt*dc4 , dt*dc5 , dt*dc6 , dt*dc7 ) );
	}


	/*
	 * Calculates Dense-Output for Dopri5
	 *
	 * See Hairer, Norsett, Wanner: Solving Ordinary Differential Equations, Nonstiff Problems. I, p.191/192
	 *
	 * y(t+theta) = y(t) + h * sum_i^7 b_i(theta) * k_i
	 *
	 * A = theta^2 * ( 3 - 2 theta )
	 * B = theta^2 * ( theta - 1 )
	 * C = theta^2 * ( theta - 1 )^2
	 * D = theta   * ( theta - 1 )^2
	 *
	 * b_1( theta ) = A * b_1 - C * X1( theta ) + D
	 * b_2( theta ) = 0
	 * b_3( theta ) = A * b_3 + C * X3( theta )
	 * b_4( theta ) = A * b_4 - C * X4( theta )
	 * b_5( theta ) = A * b_5 + C * X5( theta )
	 * b_6( theta ) = A * b_6 - C * X6( theta )
	 * b_7( theta ) = B + C * X7( theta )
	 *
	 * An alternative Method is described in:
	 *
	 * www-m2.ma.tum.de/homepages/simeon/numerik3/kap3.ps
	 */
	template< class StateOut , class StateIn1 , class DerivIn1 , class StateIn2 , class DerivIn2 >
	void calc_state( const time_type &t , StateOut &x ,
			const StateIn1 &x_old , const DerivIn1 &deriv_old , const time_type &t_old ,
			const StateIn2 &x_new , const DerivIn2 &deriv_new , const time_type &t_new )
	{
		const value_type b1 = static_cast<value_type> ( 35.0 ) / static_cast<value_type>( 384.0 );
		const value_type b3 = static_cast<value_type> ( 500.0 ) / static_cast<value_type>( 1113.0 );
		const value_type b4 = static_cast<value_type> ( 125.0 ) / static_cast<value_type>( 192.0 );
		const value_type b5 = static_cast<value_type> ( -2187.0 ) / static_cast<value_type>( 6784.0 );
		const value_type b6 = static_cast<value_type> ( 11.0 ) / static_cast<value_type>( 84.0 );

		time_type dt = ( t_new - t_old );
		value_type theta = ( t - t_old ) / dt;
		value_type X1 = static_cast< value_type >( 5.0 ) * ( static_cast< value_type >( 2558722523.0 ) - static_cast< value_type >( 31403016.0 ) * theta ) / static_cast< value_type >( 11282082432.0 );
		value_type X3 = static_cast< value_type >( 100.0 ) * ( static_cast< value_type >( 882725551.0 ) - static_cast< value_type >( 15701508.0 ) * theta ) / static_cast< value_type >( 32700410799.0 );
		value_type X4 = static_cast< value_type >( 25.0 ) * ( static_cast< value_type >( 443332067.0 ) - static_cast< value_type >( 31403016.0 ) * theta ) / static_cast< value_type >( 1880347072.0 ) ;
		value_type X5 = static_cast< value_type >( 32805.0 ) * ( static_cast< value_type >( 23143187.0 ) - static_cast< value_type >( 3489224.0 ) * theta ) / static_cast< value_type >( 199316789632.0 );
		value_type X6 = static_cast< value_type >( 55.0 ) * ( static_cast< value_type >( 29972135.0 ) - static_cast< value_type >( 7076736.0 ) * theta ) / static_cast< value_type >( 822651844.0 );
		value_type X7 = static_cast< value_type >( 10.0 ) * ( static_cast< value_type >( 7414447.0 ) - static_cast< value_type >( 829305.0 ) * theta ) / static_cast< value_type >( 29380423.0 );

		value_type theta_m_1 = theta - static_cast< value_type >( 1.0 );
		value_type theta_sq = theta * theta;
		value_type A = theta_sq * ( static_cast< value_type >( 3.0 ) - static_cast< value_type >( 2.0 ) * theta );
		value_type B = theta_sq * theta_m_1;
		value_type C = theta_sq * theta_m_1 * theta_m_1;
		value_type D = theta * theta_m_1 * theta_m_1;

		value_type b1_theta = A * b1 - C * X1 + D;
		value_type b3_theta = A * b3 + C * X3;
		value_type b4_theta = A * b4 - C * X4;
		value_type b5_theta = A * b5 + C * X5;
		value_type b6_theta = A * b6 - C * X6;
		value_type b7_theta = B + C * X7;

//		const state_type &k1 = *m_old_deriv;
//		const state_type &k3 = dopri5().m_k3;
//		const state_type &k4 = dopri5().m_k4;
//		const state_type &k5 = dopri5().m_k5;
//		const state_type &k6 = dopri5().m_k6;
//		const state_type &k7 = *m_current_deriv;

		typename algebra_type::for_each8()( x , x_old , deriv_old , m_k3 , m_k4 , m_k5 , m_k6 , deriv_new ,
			typename operations_type::template scale_sum7< value_type , time_type , time_type , time_type , time_type , time_type , time_type >( 1.0 , dt * b1_theta , dt * b3_theta , dt * b4_theta , dt * b5_theta , dt * b6_theta , dt * b7_theta ) );
	}





	void adjust_size( const state_type &x )
	{
		m_state_adjuster.adjust_size( x );
		m_deriv_adjuster.adjust_size( x );
		stepper_base_type::adjust_size( x );
	}


private:

    size_adjuster< state_type , 1 > m_state_adjuster;
    size_adjuster< deriv_type , 5 > m_deriv_adjuster;

    state_type m_x_tmp;
    deriv_type m_k2 , m_k3 , m_k4 , m_k5 , m_k6 ;

};


} // odeint
} // numeric
} // boost

#endif //BOOST_BOOST_NUMERIC_ODEINT_STEPPER_EXPLICIT_ERROR_DOPRI5_HPP_INCLUDED