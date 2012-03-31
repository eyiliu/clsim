/**
 * Copyright (c) 2011, 2012
 * Claudio Kopper <claudio.kopper@icecube.wisc.edu>
 * and the IceCube Collaboration <http://www.icecube.wisc.edu>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *
 * $Id$
 *
 * @file I3CLSimRandomValue.cxx
 * @version $Revision$
 * @date $Date$
 * @author Claudio Kopper
 */

#include <sstream>

#include <clsim/random_value/I3CLSimRandomValue.h>
#include <clsim/random_value/I3CLSimRandomValueHenyeyGreenstein.h>
#include <clsim/random_value/I3CLSimRandomValueRayleighScatteringCosAngle.h>
#include <clsim/random_value/I3CLSimRandomValueInterpolatedDistribution.h>
#include <clsim/random_value/I3CLSimRandomValueSimplifiedLiu.h>
#include <clsim/random_value/I3CLSimRandomValueMixed.h>
#include <clsim/random_value/I3CLSimRandomValueApplyFunction.h>
#include <clsim/random_value/I3CLSimRandomValueWlenCherenkovNoDispersion.h>

#ifdef NO_PYTHON_DATACLASS_SUITE
#include "icetray_python_backports/std_vector_indexing_suite.hpp"
#else
#include <icetray/python/std_vector_indexing_suite.hpp>
#endif

#include <boost/preprocessor/seq.hpp>
#include "const_ptr_helpers.h"


using namespace boost::python;
namespace bp = boost::python;

struct I3CLSimRandomValueWrapper : I3CLSimRandomValue, bp::wrapper<I3CLSimRandomValue>
{
    // pure virtual
    virtual bool OpenCLFunctionWillOnlyUseASingleRandomNumber() const 
    {
        return this->get_override("OpenCLFunctionWillOnlyUseASingleRandomNumber")();
    }
    
    virtual std::string GetOpenCLFunction(const std::string &functionName,
                                          const std::string &functionArgs,
                                          const std::string &functionArgsToCall,
                                          const std::string &uniformRandomCall_co,
                                          const std::string &uniformRandomCall_oc) const 
    {
        return this->get_override("GetOpenCLFunction")(functionName,
                                                       functionArgs,
                                                       functionArgsToCall,
                                                       uniformRandomCall_co,
                                                       uniformRandomCall_oc);
    }
    
    virtual bool CompareTo(const I3CLSimRandomValue &other) const 
    {
        return this->get_override("CompareTo")(other);
    }
    
};

bool I3CLSimRandomValue_equalWrap(const I3CLSimRandomValue &a, const I3CLSimRandomValue &b)
{
    return a==b;
}

void register_I3CLSimRandomValue()
{
    {
        bp::scope I3CLSimRandomValue_scope = 
        bp::class_<I3CLSimRandomValueWrapper, boost::shared_ptr<I3CLSimRandomValueWrapper>, boost::noncopyable>("I3CLSimRandomValue", no_init)
        .def("OpenCLFunctionWillOnlyUseASingleRandomNumber", bp::pure_virtual(&I3CLSimRandomValue::OpenCLFunctionWillOnlyUseASingleRandomNumber))
        .def("GetOpenCLFunction", bp::pure_virtual(&I3CLSimRandomValue::GetOpenCLFunction))
        .def("CompareTo", bp::pure_virtual(&I3CLSimRandomValue::CompareTo))
        .def("__eq__", &I3CLSimRandomValue_equalWrap)
        ;
    }
    
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueWrapper>, shared_ptr<const I3CLSimRandomValue> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueWrapper>, shared_ptr<I3CLSimRandomValue> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueWrapper>, shared_ptr<const I3CLSimRandomValueWrapper> >();
    utils::register_const_ptr<I3CLSimRandomValue>();
    
    // Henyey-Greenstein
    {
        bp::class_<
        I3CLSimRandomValueHenyeyGreenstein, 
        boost::shared_ptr<I3CLSimRandomValueHenyeyGreenstein>, 
        bases<I3CLSimRandomValue>,
        boost::noncopyable
        >
        (
         "I3CLSimRandomValueHenyeyGreenstein",
         bp::init<
         double
         >(
           (
            bp::arg("meanCosine")
           )
          )
        )
        ;
    }
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueHenyeyGreenstein>, shared_ptr<const I3CLSimRandomValueHenyeyGreenstein> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueHenyeyGreenstein>, shared_ptr<I3CLSimRandomValue> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueHenyeyGreenstein>, shared_ptr<const I3CLSimRandomValue> >();
    utils::register_const_ptr<I3CLSimRandomValueHenyeyGreenstein>();

    
    // Rayleigh scattering
    {
        bp::class_<
        I3CLSimRandomValueRayleighScatteringCosAngle, 
        boost::shared_ptr<I3CLSimRandomValueRayleighScatteringCosAngle>, 
        bases<I3CLSimRandomValue>,
        boost::noncopyable
        >
        (
         "I3CLSimRandomValueRayleighScatteringCosAngle",
         bp::init<>()
        )
        ;
    }
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueRayleighScatteringCosAngle>, shared_ptr<const I3CLSimRandomValueRayleighScatteringCosAngle> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueRayleighScatteringCosAngle>, shared_ptr<I3CLSimRandomValue> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueRayleighScatteringCosAngle>, shared_ptr<const I3CLSimRandomValue> >();
    utils::register_const_ptr<I3CLSimRandomValueRayleighScatteringCosAngle>();

    
    // simplified Liu
    {
        bp::class_<
        I3CLSimRandomValueSimplifiedLiu, 
        boost::shared_ptr<I3CLSimRandomValueSimplifiedLiu>, 
        bases<I3CLSimRandomValue>,
        boost::noncopyable
        >
        (
         "I3CLSimRandomValueSimplifiedLiu",
         bp::init<
         double
         >(
           (
            bp::arg("meanCosine")
            )
           )
         )
        ;
    }
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueSimplifiedLiu>, shared_ptr<const I3CLSimRandomValueSimplifiedLiu> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueSimplifiedLiu>, shared_ptr<I3CLSimRandomValue> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueSimplifiedLiu>, shared_ptr<const I3CLSimRandomValue> >();
    utils::register_const_ptr<I3CLSimRandomValueSimplifiedLiu>();

    
    // table of (x,value) pairs
    {
        bp::class_<
        I3CLSimRandomValueInterpolatedDistribution, 
        boost::shared_ptr<I3CLSimRandomValueInterpolatedDistribution>, 
        bases<I3CLSimRandomValue>,
        boost::noncopyable
        >
        (
         "I3CLSimRandomValueInterpolatedDistribution",
         bp::init<
         const std::vector<double> &,
         const std::vector<double> &
         >(
           (
            bp::arg("x"),
            bp::arg("y")
           )
          )
         )
        .def(init<double, double, const std::vector<double> & >
             (
              (
               bp::arg("xFirst"),
               bp::arg("xSpacing"),
               bp::arg("y")
               )
              )
             )

        ;
    }
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueInterpolatedDistribution>, shared_ptr<const I3CLSimRandomValueInterpolatedDistribution> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueInterpolatedDistribution>, shared_ptr<I3CLSimRandomValue> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueInterpolatedDistribution>, shared_ptr<const I3CLSimRandomValue> >();
    utils::register_const_ptr<I3CLSimRandomValueInterpolatedDistribution>();

    
    // mix of two distributions
    {
        bp::class_<
        I3CLSimRandomValueMixed, 
        boost::shared_ptr<I3CLSimRandomValueMixed>, 
        bases<I3CLSimRandomValue>,
        boost::noncopyable
        >
        (
         "I3CLSimRandomValueMixed",
         bp::init<
         double,
         I3CLSimRandomValueConstPtr,
         I3CLSimRandomValueConstPtr
         >(
           (
            bp::arg("fractionOfFirstDistribution"),
            bp::arg("firstDistribution"),
            bp::arg("secondDistribution")
            )
           )
         )
        ;
    }
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueMixed>, shared_ptr<const I3CLSimRandomValueMixed> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueMixed>, shared_ptr<I3CLSimRandomValue> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueMixed>, shared_ptr<const I3CLSimRandomValue> >();
    utils::register_const_ptr<I3CLSimRandomValueMixed>();

    
    // apply function to generated random value
    {
        bp::class_<
        I3CLSimRandomValueApplyFunction, 
        boost::shared_ptr<I3CLSimRandomValueApplyFunction>, 
        bases<I3CLSimRandomValue>,
        boost::noncopyable
        >
        (
         "I3CLSimRandomValueApplyFunction",
         bp::init<
         I3CLSimRandomValuePtr,
         const std::string &
         >(
           (
            bp::arg("randomDistUsed"),
            bp::arg("functionName")
            )
           )
         )
        ;
    }
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueApplyFunction>, shared_ptr<const I3CLSimRandomValueApplyFunction> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueApplyFunction>, shared_ptr<I3CLSimRandomValue> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueApplyFunction>, shared_ptr<const I3CLSimRandomValue> >();
    utils::register_const_ptr<I3CLSimRandomValueApplyFunction>();

    
    // wavelength distributed according to a Cherenkov spectrum (no dispersion)
    {
        bp::class_<
        I3CLSimRandomValueWlenCherenkovNoDispersion, 
        boost::shared_ptr<I3CLSimRandomValueWlenCherenkovNoDispersion>, 
        bases<I3CLSimRandomValue>,
        boost::noncopyable
        >
        (
         "I3CLSimRandomValueWlenCherenkovNoDispersion",
         bp::init<
         double,
         double
         >(
           (
            bp::arg("fromWlen"),
            bp::arg("toWlen")
            )
           )
         )
        ;
    }
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueWlenCherenkovNoDispersion>, shared_ptr<const I3CLSimRandomValueWlenCherenkovNoDispersion> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueWlenCherenkovNoDispersion>, shared_ptr<I3CLSimRandomValue> >();
    bp::implicitly_convertible<shared_ptr<I3CLSimRandomValueWlenCherenkovNoDispersion>, shared_ptr<const I3CLSimRandomValue> >();
    utils::register_const_ptr<I3CLSimRandomValueWlenCherenkovNoDispersion>();
    
    {
        typedef std::vector<I3CLSimRandomValueConstPtr> Series;
        bp::class_<Series, shared_ptr<Series> >("I3CLSimRandomValuePtrSeries")
            .def(bp::std_vector_indexing_suite<Series>())
        ;
    }

}
