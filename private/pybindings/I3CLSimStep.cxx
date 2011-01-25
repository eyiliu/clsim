//
//   Copyright (c) 2011  Claudio Kopper
//   
//   $Id$
//
//   This file is part of the IceTray module g4sim-interface.
//
//   g4sim-intrface is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   IceTray is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <sstream>

#include <icetray/I3Units.h>

#include <clsim/I3CLSimStep.h>
#include <boost/preprocessor/seq.hpp>
#include <icetray/python/std_vector_indexing_suite.hpp>

using namespace boost::python;

static std::string 
i3clsimstep_prettyprint(const I3CLSimStep& s)
{
    I3DirectionPtr dir = s.GetDir();
    if (!dir) dir=I3DirectionPtr(new I3Direction());
    
    std::ostringstream oss;
    oss << "[ I3CLSimStep id : " << s.GetID() << std::endl
        << "     pos (x,y,z) : [" << s.GetPosX()/I3Units::m << ", " << s.GetPosY()/I3Units::m << ", " << s.GetPosZ()/I3Units::m << "]m" << std::endl
        << " dir (theta,phi) : [" << s.GetDirTheta()/I3Units::rad << ", " << s.GetDirPhi()/I3Units::rad << "]rad, [" << s.GetDirTheta()/I3Units::deg << ", " << s.GetDirPhi()/I3Units::deg << "]deg" << std::endl
        << "   dir (zen,azi) : [" << dir->GetZenith()/I3Units::rad << ", " << dir->GetAzimuth()/I3Units::rad << "]rad, [" << dir->GetZenith()/I3Units::deg << ", " << dir->GetAzimuth()/I3Units::deg << "]deg" << std::endl
        << "     dir (x,y,z) : [" << dir->GetX() << ", " << dir->GetY() << ", " << dir->GetZ() << "]" << std::endl
        << "            time : " << s.GetTime()/I3Units::ns << "ns" << std::endl
        << "          length : " << s.GetLength()/I3Units::mm << "mm" << std::endl
        << "             num : " << s.GetNumPhotons() << std::endl
        << "          weight : " << s.GetWeight() << std::endl
        << "            beta : " << s.GetBeta() << std::endl
        << "]" ;
    
    return oss.str();
}


void register_I3CLSimStep()
{
    {
        void (I3CLSimStep::* SetDir_oneary)(const I3Direction&) = &I3CLSimStep::SetDir; 
        void (I3CLSimStep::* SetDir_threeary)(const double &x, const double &y, const double &z) = &I3CLSimStep::SetDir;
        
        scope clsimstep_scope = 
        class_<I3CLSimStep, boost::shared_ptr<I3CLSimStep> >("I3CLSimStep")
        .add_property("x", &I3CLSimStep::GetPosX, &I3CLSimStep::SetPosX)
        .add_property("y", &I3CLSimStep::GetPosY, &I3CLSimStep::SetPosY)
        .add_property("z", &I3CLSimStep::GetPosZ, &I3CLSimStep::SetPosZ)
        .add_property("time", &I3CLSimStep::GetTime, &I3CLSimStep::SetTime)

        .add_property("theta", &I3CLSimStep::GetDirTheta, &I3CLSimStep::SetDirTheta)
        .add_property("phi", &I3CLSimStep::GetDirPhi, &I3CLSimStep::SetDirPhi)
        .add_property("length", &I3CLSimStep::GetLength, &I3CLSimStep::SetLength)
        .add_property("beta", &I3CLSimStep::GetBeta, &I3CLSimStep::SetBeta)

        .add_property("num", &I3CLSimStep::GetNumPhotons, &I3CLSimStep::SetNumPhotons)
        .add_property("weight", &I3CLSimStep::GetWeight, &I3CLSimStep::SetWeight)
        .add_property("id", &I3CLSimStep::GetID, &I3CLSimStep::SetID)

        .add_property("pos", &I3CLSimStep::GetPos, &I3CLSimStep::SetPos)
        .add_property("dir", &I3CLSimStep::GetDir, SetDir_oneary)
        
        .def("SetDirXYZ", SetDir_threeary)
        
        .def("__str__", i3clsimstep_prettyprint)
        ;
    }
    
    class_<I3CLSimStepSeries, bases<I3FrameObject>, I3CLSimStepSeriesPtr>("I3CLSimStepSeries")
    .def(std_vector_indexing_suite<I3CLSimStepSeries>())
    ;
    
    // does not base on I3FrameObject, so register only the shared_ptr<T>-to-shared_ptr<const T> conversion
    //register_pointer_conversions<I3CLSimStep>();
    boost::python::implicitly_convertible<shared_ptr<I3CLSimStep>, shared_ptr<const I3CLSimStep> >();
    
    register_pointer_conversions<I3CLSimStepSeries>();
}