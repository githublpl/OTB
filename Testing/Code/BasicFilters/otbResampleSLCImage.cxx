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
#include "itkExceptionObject.h"

#include "otbStreamingResampleImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "itkTranslationTransform.h"

int otbResampleSLCImage(int argc, char * argv[])
{
  const char* inputFilename = argv[1];
  const char* outputFilename = argv[2];

  const unsigned int Dimension = 2;
  typedef std::complex<double> InputPixelType;
  typedef std::complex<double> OutputPixelType;
//   typedef double InterpolatorPrecisionType;

  typedef otb::Image<InputPixelType, Dimension>          InputImageType;
  typedef otb::Image<OutputPixelType, Dimension>         OutputImageType;
  typedef otb::ImageFileReader<InputImageType>           ReaderType;
  typedef otb::StreamingImageFileWriter<OutputImageType> WriterType;
  typedef itk::TranslationTransform<double, Dimension>   TransformType;
//   typedef otb::StreamingResampleImageFilter<InputImageType,OutputImageType,InterpolatorPrecisionType> StreamingResampleImageFilterType;
  typedef itk::ResampleImageFilter<InputImageType, OutputImageType> ResampleFilterType;

  // Instantiating object
  ReaderType::Pointer         reader = ReaderType::New();
  WriterType::Pointer         writer = WriterType::New();
  ResampleFilterType::Pointer resampler = ResampleFilterType::New();
  TransformType::Pointer      transform = TransformType::New();

  // Input Image
  reader->SetFileName(inputFilename);

  // Resampler connected to input image
  resampler->SetInput(reader->GetOutput());

  // Size of output resampler result
  ResampleFilterType::SizeType size;
  size[0] = 200;
  size[1] = 200;
  resampler->SetSize(size);

  // Transformation creation
  TransformType::OutputVectorType translation;
  translation[0] = 10;
  translation[1] = 20;
  transform->SetOffset(translation);

  // Resampler is updated with new transformation (default is identity)
  resampler->SetTransform(transform);

  // Result of resampler is written
  writer->SetInput(resampler->GetOutput());
  writer->SetNumberOfStreamDivisions(1);
  writer->SetFileName(outputFilename);

  writer->Update();

  return EXIT_SUCCESS;
}
