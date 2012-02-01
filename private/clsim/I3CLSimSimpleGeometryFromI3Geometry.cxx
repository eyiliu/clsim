#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#include <icetray/I3Units.h>
#include "clsim/I3CLSimSimpleGeometryFromI3Geometry.h"

#include <stdexcept>
#include <limits>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

// TODO: these defaults are IceCube-specific!
const std::set<int> I3CLSimSimpleGeometryFromI3Geometry::default_ignoreStrings;
const std::set<unsigned int> I3CLSimSimpleGeometryFromI3Geometry::default_ignoreDomIDs;
const std::set<std::string> I3CLSimSimpleGeometryFromI3Geometry::default_ignoreSubdetectors;
const int32_t I3CLSimSimpleGeometryFromI3Geometry::default_ignoreStringIDsSmallerThan = 1;
const int32_t I3CLSimSimpleGeometryFromI3Geometry::default_ignoreStringIDsLargerThan = std::numeric_limits<int32_t>::max();
const uint32_t I3CLSimSimpleGeometryFromI3Geometry::default_ignoreDomIDsSmallerThan = 1;
const uint32_t I3CLSimSimpleGeometryFromI3Geometry::default_ignoreDomIDsLargerThan = 60;
const bool I3CLSimSimpleGeometryFromI3Geometry::default_splitIntoPartsAccordingToPosition=false;
const bool I3CLSimSimpleGeometryFromI3Geometry::default_useHardcodedDeepCoreSubdetector=false;

// some helpers
namespace {
    
    
}


I3CLSimSimpleGeometryFromI3Geometry::
I3CLSimSimpleGeometryFromI3Geometry(double OMRadius,
                                    const I3GeometryConstPtr &geometry,
                                    const std::set<int> &ignoreStrings,
                                    const std::set<unsigned int> &ignoreDomIDs,
                                    const std::set<std::string> &ignoreSubdetectors,
                                    int32_t ignoreStringIDsSmallerThan,
                                    int32_t ignoreStringIDsLargerThan,
                                    uint32_t ignoreDomIDsSmallerThan,
                                    uint32_t ignoreDomIDsLargerThan,
                                    bool splitIntoPartsAccordingToPosition,
                                    bool useHardcodedDeepCoreSubdetector)
:
OMRadius_(OMRadius),
splitIntoPartsAccordingToPosition_(splitIntoPartsAccordingToPosition),
ignoreStrings_(ignoreStrings),
ignoreDomIDs_(ignoreDomIDs),
ignoreSubdetectors_(ignoreSubdetectors),
ignoreStringIDsSmallerThan_(ignoreStringIDsSmallerThan),
ignoreStringIDsLargerThan_(ignoreStringIDsLargerThan),
ignoreDomIDsSmallerThan_(ignoreDomIDsSmallerThan),
ignoreDomIDsLargerThan_(ignoreDomIDsLargerThan),
useHardcodedDeepCoreSubdetector_(useHardcodedDeepCoreSubdetector)
{
    if (!geometry) throw std::runtime_error("Received NULL geometry pointer!");
    
    log_debug("Ignoring StringNum<%" PRIi32 ", StringNum>%" PRIi32 ", OMNum<%" PRIu32 ", OMNum>%" PRIu32 ".",
              ignoreStringIDsSmallerThan, ignoreStringIDsLargerThan,
              ignoreDomIDsSmallerThan, ignoreDomIDsLargerThan);
    
#ifdef HAS_MULTIPMT_SUPPORT
    // split the detector into different parts
    const double EPSILON=1.*I3Units::m;

    std::map<OMKey, unsigned int> omKeyToDetectorPart;
    std::map<int64_t, std::vector<std::pair<double, double> > > modifiedStringNumToXYPosList;
    unsigned int numParts;
    
    unsigned int maxOMsPerFloor = geometry->layout.GetMaxOMCountPerFloor();
    if (maxOMsPerFloor <= 1)
    {
        // only one OM per floor.
        BOOST_FOREACH(const I3OMGeoMap::value_type &i, geometry->omgeo)
        {
            const OMKey &key = i.first;
            omKeyToDetectorPart.insert(std::make_pair(key, 0)); // all OMs are in part #0
            numParts=1;
        }
    }
    else
    {
        numParts=0;
        
        if (splitIntoPartsAccordingToPosition_)
        {
            BOOST_FOREACH(const I3OMGeoMap::value_type &i, geometry->omgeo)
            {
                const OMKey &key = i.first;
                const unsigned int floorIndex = geometry->layout.GetFloorIndex(key);

                const int64_t modifiedStringNum = key.GetString() * static_cast<int64_t>(maxOMsPerFloor) + floorIndex;
                std::map<int64_t, std::vector<std::pair<double, double> > >::iterator it = modifiedStringNumToXYPosList.find(modifiedStringNum);
                if (it==modifiedStringNumToXYPosList.end())
                    it = modifiedStringNumToXYPosList.insert(std::make_pair(modifiedStringNum, std::vector<std::pair<double, double> >())).first;
                std::vector<std::pair<double, double> > &XYPosList = it->second;

                const I3OMGeo &geo = i.second;

                bool fitFound=false;
                for (unsigned int i=0;i<XYPosList.size();++i)
                {
                    std::pair<double, double> &XYPos = XYPosList[i];
                    
                    if ((std::abs(XYPos.first - geo.position.GetX()) < EPSILON) &&
                        (std::abs(XYPos.second - geo.position.GetY()) < EPSILON))
                    {
                        // it fits!
                        
                        fitFound=true;
                        omKeyToDetectorPart.insert(std::make_pair(key, (i*maxOMsPerFloor)+floorIndex));
                        break;
                    }
                }
                
                if (!fitFound)
                {
                    // make a new entry..
                    
                    XYPosList.push_back(std::make_pair(geo.position.GetX(), geo.position.GetY()));
                    unsigned int new_index = static_cast<unsigned int>(XYPosList.size()-1);
                    omKeyToDetectorPart.insert(std::make_pair(key, (new_index*maxOMsPerFloor)+floorIndex));
                    if ((new_index*maxOMsPerFloor)+floorIndex+1 > numParts) numParts=(new_index*maxOMsPerFloor)+floorIndex+1;
                }
            }
        } else {
            BOOST_FOREACH(const I3OMGeoMap::value_type &i, geometry->omgeo)
            {
                const OMKey &key = i.first;
                const unsigned int floorIndex = geometry->layout.GetFloorIndex(key);
                omKeyToDetectorPart.insert(std::make_pair(key, floorIndex));
            }
            
            numParts = maxOMsPerFloor;
        }
    }
    
    log_info("The detector consists of %u parts.", numParts);
#endif
    
    numOMs_=0;
    BOOST_FOREACH(const I3OMGeoMap::value_type &i, geometry->omgeo)
    {
        const OMKey &key = i.first;
        const I3OMGeo &geo = i.second;
        
        int32_t string=key.GetString();
        uint32_t dom=key.GetOM();

#ifdef HAS_MULTIPMT_SUPPORT
        std::string subdetectorName = geo.subdetector;
#else
        std::string subdetectorName;
        switch (geo.omtype)
        {
            case I3OMGeo::UnknownType: subdetectorName = "UnknownType"; break;
            case I3OMGeo::AMANDA: subdetectorName = "AMANDA"; break;
            case I3OMGeo::IceCube: subdetectorName = "IceCube"; break;
            case I3OMGeo::IceTop: subdetectorName = "IceTop"; break;
            default: subdetectorName = "(unknown)"; break;
        }
#endif
        
        if (useHardcodedDeepCoreSubdetector_) {
            // special hack for DeepCore
            if (subdetectorName=="IceCube")
            {
                if ((string>=79) && (string<=86)) // these are the DeepCore strings
                {
                    if (geo.position.GetZ()>-30.*I3Units::m) // z=30m is about halfway between the upper and lower parts of DeepCore
                        subdetectorName="DeepCoreUpper";
                    else
                        subdetectorName="DeepCoreLower";
                }
				else if (string > 86)
				{
					subdetectorName="PINGU";
				}
            }
        }
        
        if ((string < ignoreStringIDsSmallerThan_) ||
            (string > ignoreStringIDsLargerThan_) ||
            (dom < ignoreDomIDsSmallerThan_) ||
            (dom > ignoreDomIDsLargerThan_))
            continue;

        if (ignoreStrings_.count(string)!=0) continue;
        if (ignoreDomIDs_.count(dom)!=0) continue;
        if (ignoreSubdetectors_.count(subdetectorName)!=0) continue;

#ifdef HAS_MULTIPMT_SUPPORT
        // assign different subdetectors for each floor index.
        // this should make it easier to find a geometry binning later on.
        // Should be irrelevant for IceCube/KM3NeT string detectors, 
        // only Antares (3 OMs per floor) and KM3NeT bar detectors (2 OMs per
        // floor) should be affected. -ck
        std::map<OMKey, unsigned int>::const_iterator it = omKeyToDetectorPart.find(key);
        if (it == omKeyToDetectorPart.end()) log_fatal("internal error: OMKey not in detector part list");
        subdetectorName = subdetectorName + "_" + boost::lexical_cast<std::string>(it->second);
#endif
        
        stringIDs_.push_back(string);
        domIDs_.push_back(dom);
        posX_.push_back(geo.position.GetX());
        posY_.push_back(geo.position.GetY());
        posZ_.push_back(geo.position.GetZ());
        subdetectors_.push_back(subdetectorName);

        ++numOMs_;
    }
    
}

I3CLSimSimpleGeometryFromI3Geometry::
~I3CLSimSimpleGeometryFromI3Geometry()
{
    
}
