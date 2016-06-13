/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbSamplingRateCalculator.h"
#include "otbOGRDataToSamplePositionFilter.h"
#include "otbStatisticsXMLFileReader.h"
#include "otbRandomSampler.h"

namespace otb
{
namespace Wrapper
{

class SampleSelection : public Application
{
public:
  /** Standard class typedefs. */
  typedef SampleSelection        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SampleSelection, otb::Application);

  /** typedef */
  typedef otb::OGRDataToSamplePositionFilter<
    FloatVectorImageType,
    UInt8ImageType,
    otb::PeriodicSampler>                           PeriodicSamplerType;
  typedef otb::OGRDataToSamplePositionFilter<
    FloatVectorImageType,
    UInt8ImageType,
    otb::RandomSampler>                             RandomSamplerType;
  typedef otb::SamplingRateCalculator               RateCalculatorType;
  
  typedef std::map<std::string, unsigned long>      ClassCountMapType;
  typedef RateCalculatorType::MapRateType           MapRateType;
  typedef itk::VariableLengthVector<float> MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType> XMLReaderType;

private:
  SampleSelection()
    {
    m_Periodic = PeriodicSamplerType::New();
    m_Random = RandomSamplerType::New();
    m_ReaderStat = XMLReaderType::New();
    m_RateCalculator = RateCalculatorType::New();
    }

  void DoInit()
  {
    SetName("SampleSelection");
    SetDescription("Selects samples from a training vector data set.");

    // Documentation
    SetDocName("Polygon Class Statistics");
    SetDocLongDescription("The application selects a set of samples from geometries "
      "intended for training (they should have a field giving the associated "
      "class). \n\nFirst of all, the geometries must be analyzed by the PolygonClassStatistics application "
      "to compute statistics about the geometries, which are summarized in an xml file. "
      "\nThen, this xml file must be given as input to this application (parameter instats).\n\n"
      "The input support image and the input training vectors shall be given in "
      "parameters 'in' and 'vec' respectively. Only the sampling grid (origin, size, spacing)"
      "will be read in the input image.\n"
      "There are several strategies to select samples (parameter strat) :\n"
      "  - smallest (default) : select the same number of sample in each class\n" 
      "    so that the smallest one is fully sampled.\n"
      "  - constant : select the same number of samples N in each class\n" 
      "    (with N below or equal to the size of the smallest class).\n"
      "  - byclass : set the required number for each class manually, with an input CSV file\n" 
      "    (first column is class name, second one is the required samples number).\n"
      "There is also a choice on the sampling type to performs :\n"
      "  - periodic : select samples uniformly distributed\n"
      "  - random : select samples randomly distributed\n"
      "Once the strategy and type are selected, the application outputs samples positions"
      "(parameter out).\n\n"
      
      "The other parameters to look at are :\n"
      "  - layer : index specifying from which layer to pick geometries.\n"
      "  - field : set the field name containing the class.\n"
      "  - mask : an optional raster mask can be used to discard samples.\n"
      "  - outrates : allows to output a CSV file that summarizes the sampling rates for each class.\n"
      
      "\nAs with the PolygonClassStatistics application, different types  of geometry are supported : "
      "polygons, lines, points. \nThe behavior of this application is different for each type of geometry :\n"
      "  - polygon: select points whose center is inside the polygon\n"
      "  - lines  : select points intersecting the line\n"
      "  - points : select closest point to the provided point\n");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage,  "in",   "InputImage");
    SetParameterDescription("in", "Support image that will be classified");

    AddParameter(ParameterType_InputImage,  "mask",   "InputMask");
    SetParameterDescription("mask", "Validity mask (only pixels corresponding to a mask value greater than 0 will be used for statistics)");
    MandatoryOff("mask");

    AddParameter(ParameterType_InputFilename, "vec", "Input vectors");
    SetParameterDescription("vec","Input geometries to analyse");

    AddParameter(ParameterType_OutputFilename, "out", "Output vectors");
    SetParameterDescription("out","Output resampled geometries");

    AddParameter(ParameterType_InputFilename, "instats", "Input Statistics");
    SetParameterDescription("instats","Input file storing statistics (XML format)");

    AddParameter(ParameterType_OutputFilename, "outrates", "Output rates");
    SetParameterDescription("outrates","Output rates (CSV formated)");
    MandatoryOff("outrates");

    AddParameter(ParameterType_Choice, "sampler", "Sampler type");
    SetParameterDescription("sampler", "Type of sampling (periodic, pattern based, random)");

    AddChoice("sampler.periodic","Periodic sampler");
    SetParameterDescription("sampler.periodic","Takes samples regularly spaced");

    AddChoice("sampler.random","Random sampler");
    SetParameterDescription("sampler.random","The positions to select are randomly shuffled.");

    AddParameter(ParameterType_Choice, "strat", "Sampling strategy");

    AddChoice("strat.byclass","Set samples count for each class");
    SetParameterDescription("strat.byclass","Set samples count for each class");

    AddParameter(ParameterType_InputFilename, "strat.byclass.in", "Number of samples by class");
    SetParameterDescription("strat.byclass.in", "Number of samples by class "
      "(CSV format with class name in 1st column and required samples in the 2nd.");

    AddChoice("strat.constant","Set the same samples counts for all classes");
    SetParameterDescription("strat.constant","Set the same samples counts for all classes");

    AddParameter(ParameterType_Int, "strat.constant.nb", "Number of samples for all classes");
    SetParameterDescription("strat.constant.nb", "Number of samples for all classes");

    AddChoice("strat.smallest","Set same number of samples for all classes, with the smallest class fully sampled");
    SetParameterDescription("strat.smallest","Set same number of samples for all classes, with the smallest class fully sampled");

    AddChoice("strat.all","Take all samples");
    SetParameterDescription("strat.all","Take all samples");

    // Default strategy : smallest
    SetParameterString("strat","smallest");

    AddParameter(ParameterType_String, "field", "Field Name");
    SetParameterDescription("field","Name of the field carrying the class name in the input vectors.");
    MandatoryOff("field");
    SetParameterString("field", "class");

    AddParameter(ParameterType_Int, "layer", "Layer Index");
    SetParameterDescription("layer", "Layer index to read in the input vector file.");
    MandatoryOff("layer");
    SetDefaultParameterInt("layer",0);

    AddRAMParameter();

    AddRANDParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "support_image.tif");
    SetDocExampleParameterValue("vec", "variousVectors.sqlite");
    SetDocExampleParameterValue("field", "label");
    SetDocExampleParameterValue("instats","apTvClPolygonClassStatisticsOut.xml");
    SetDocExampleParameterValue("out","resampledVectors.sqlite");
  }

  void DoUpdateParameters()
  {
  }

  ClassCountMapType ReadRequiredSamples(std::string filename)
    {
    ClassCountMapType output;
    std::ifstream ifs(filename.c_str());

    if (ifs)
      {
      std::string line;
      std::string sep("");
    
      while(!ifs.eof())
        {
        std::getline(ifs,line);
        if (line.empty()) continue;
        std::string::size_type pos = line.find_first_not_of(" \t");
        if (pos != std::string::npos && line[pos] == '#') continue;
        
        if (sep.size() == 0)
          {
          // Try to detect the separator
          std::string separators("\t;,");
          for (unsigned int k=0 ; k<separators.size() ; k++)
            {
            std::vector<itksys::String> words = itksys::SystemTools::SplitString(line,separators[k]);
            if (words.size() >= 2)
              {
              sep.push_back(separators[k]);
              break;
              }
            }
          if (sep.size() == 0) continue;
          }
        // parse the line
        std::vector<itksys::String> parts = itksys::SystemTools::SplitString(line,sep[0]);
        if (parts.size() >= 2)
          {
          std::string::size_type pos1 = parts[0].find_first_not_of(" \t");
          std::string::size_type pos2 = parts[0].find_last_not_of(" \t");
          std::string::size_type pos3 = parts[1].find_first_not_of(" \t");
          std::string::size_type pos4 = parts[1].find_last_not_of(" \t");
          if (pos1 != std::string::npos && pos3 != std::string::npos)
            {
            std::string name = parts[0].substr(pos1, pos2 - pos1 + 1);
            std::string value = parts[1].substr(pos3, pos4 - pos3 + 1);
            output[name] = boost::lexical_cast<unsigned long>(value);
            }
          }
        }
      ifs.close();
      }
    else
      {
      otbAppLogFATAL(<< " Couldn't open " << filename);
      }
    return output;
    }

  void DoExecute()
    {
    // Clear state
    m_RateCalculator->ClearRates();
    m_Periodic->GetFilter()->ClearOutputs();
    m_Random->GetFilter()->ClearOutputs();
    
    m_ReaderStat->SetFileName(this->GetParameterString("instats"));
    ClassCountMapType classCount = m_ReaderStat->GetStatisticMapByName<ClassCountMapType>("samplesPerClass");
    m_RateCalculator->SetClassCount(classCount);
    
    switch (this->GetParameterInt("strat"))
      {
      // byclass
      case 0:
        {
        otbAppLogINFO("Sampling strategy : set number of samples for each class");
        ClassCountMapType requiredCount = 
          this->ReadRequiredSamples(this->GetParameterString("strat.byclass.in"));
        m_RateCalculator->SetNbOfSamplesByClass(requiredCount);
        }
      break;
      // constant
      case 1:
        {
        otbAppLogINFO("Sampling strategy : set a constant number of samples for all classes");
        m_RateCalculator->SetNbOfSamplesAllClasses(GetParameterInt("strat.constant.nb"));
        }
      break;
      // smallest class
      case 2:
        {
        otbAppLogINFO("Sampling strategy : fit the number of samples based on the smallest class");
        m_RateCalculator->SetMinimumNbOfSamplesByClass();
        }
      break;
      // all samples
      case 3:
        {
        otbAppLogINFO("Sampling strategy : take all samples");
        m_RateCalculator->SetAllSamples();
        }
      break;
      default:
        otbAppLogFATAL("Strategy mode unknown :"<<this->GetParameterString("strat"));
      break;
      }
      
    if (IsParameterEnabled("outrates") && HasValue("outrates"))
      {
      m_RateCalculator->Write(this->GetParameterString("outrates"));
      }
    
    MapRateType rates = m_RateCalculator->GetRatesByClass();
    std::ostringstream oss;
    oss << " className  requiredSamples  totalSamples  rate" << std::endl;
    MapRateType::const_iterator itRates = rates.begin();
    for(; itRates != rates.end(); ++itRates)
      {
      otb::SamplingRateCalculator::TripletType tpt = itRates->second;
      oss << itRates->first << "\t" << tpt.Required << "\t" << tpt.Tot << "\t" << tpt.Rate << std::endl;
      }
    otbAppLogINFO("Sampling rates : " << oss.str());

    // Open input geometries
    otb::ogr::DataSource::Pointer vectors =
      otb::ogr::DataSource::New(this->GetParameterString("vec"));

    // Create output dataset for sample positions
    otb::ogr::DataSource::Pointer outputSamples =
      otb::ogr::DataSource::New(this->GetParameterString("out"),otb::ogr::DataSource::Modes::Overwrite);
    
    switch (this->GetParameterInt("sampler"))
      {
      // periodic
      case 0:
        {
        m_Periodic->SetInput(this->GetParameterImage("in"));
        m_Periodic->SetOGRData(vectors);
        m_Periodic->SetOutputPositionContainerAndRates(outputSamples, rates);
        m_Periodic->SetFieldName(this->GetParameterString("field"));
        m_Periodic->SetLayerIndex(this->GetParameterInt("layer"));
        if (IsParameterEnabled("mask") && HasValue("mask"))
          {
          m_Periodic->SetMask(this->GetParameterImage<UInt8ImageType>("mask"));
          }
        m_Periodic->GetStreamer()->SetAutomaticTiledStreaming(this->GetParameterInt("ram"));
        AddProcess(m_Periodic->GetStreamer(),"Selecting positions with periodic sampler...");
        m_Periodic->Update();
        }
      break;
      // random
      case 1:
        {
        m_Random->SetInput(this->GetParameterImage("in"));
        m_Random->SetOGRData(vectors);
        m_Random->SetOutputPositionContainerAndRates(outputSamples, rates);
        m_Random->SetFieldName(this->GetParameterString("field"));
        m_Random->SetLayerIndex(this->GetParameterInt("layer"));
        if (IsParameterEnabled("mask") && HasValue("mask"))
          {
          m_Random->SetMask(this->GetParameterImage<UInt8ImageType>("mask"));
          }
        m_Random->GetStreamer()->SetAutomaticTiledStreaming(this->GetParameterInt("ram"));
        AddProcess(m_Random->GetStreamer(),"Selecting positions with random sampler...");
        m_Random->Update();
        }
      break;
      default:
        otbAppLogFATAL("Sampler type unknown :"<<this->GetParameterString("sampler"));
      break;
      }
  }

  RateCalculatorType::Pointer m_RateCalculator;
  
  PeriodicSamplerType::Pointer m_Periodic;
  RandomSamplerType::Pointer m_Random;
  
  XMLReaderType::Pointer m_ReaderStat;
};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SampleSelection)
