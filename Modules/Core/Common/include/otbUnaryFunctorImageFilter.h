/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbUnaryFunctorImageFilter_h
#define otbUnaryFunctorImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
/**
 * \class UnaryFunctorImageFilter
 * \brief Implements pixel-wise generic operation on one image.
 *
 * Add the capability to change the number of channel when operation on
 * VectorImage compared to the itk::UnaryFunctorImageFilter
 *
 * The number of channel is provided by the functor: TFunction::GetOutputSize. If
 * this number is lower or equal to zero, the behavior of the itk::UnaryFunctorImageFilter
 * remains unchanged.
 *
 * \sa itk::UnaryFunctorImageFilter
 *
 * \ingroup OTBCommon
 */
template <class TInputImage, class TOutputImage, class TFunction>
class ITK_EXPORT UnaryFunctorImageFilter : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef UnaryFunctorImageFilter                                            Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                            Pointer;
  typedef itk::SmartPointer<const Self>                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UnaryFunctorImageFilter, itk::UnaryFunctorImageFilter);

protected:
  UnaryFunctorImageFilter() {};
  ~UnaryFunctorImageFilter() override {}

  /** UnaryFunctorImageFilter can produce an image which has a different number of bands
   * than its input image.  As such, UnaryFunctorImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  void GenerateOutputInformation() override
  {
    Superclass::GenerateOutputInformation();
    typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
    outputPtr->SetNumberOfComponentsPerPixel( // propagate vector length info
      this->GetFunctor().GetOutputSize());
  }

private:
  UnaryFunctorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
