#ifndef AutoencoderModel_h
#define AutoencoderModel_h

#include "DimensionalityReductionModel.h"


namespace otb
{
template <class TInputValue, class AutoencoderType>
class ITK_EXPORT AutoencoderModel: public  DimensionalityReductionModel<TInputValue,TInputValue>   
{

public:
	
	typedef AutoencoderModel Self;
	typedef DimensionalityReductionModel<TInputValue,TInputValue> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef typename Superclass::InputValueType InputValueType;
	typedef typename Superclass::InputSampleType InputSampleType;
	typedef typename Superclass::InputListSampleType InputListSampleType;
	typedef typename Superclass::TargetValueType TargetValueType;
	typedef typename Superclass::TargetSampleType TargetSampleType;
	typedef typename Superclass::TargetListSampleType TargetListSampleType;
	
	itkNewMacro(Self);
	itkTypeMacro(AutoencoderModel, DimensionalityReductionModel);

	unsigned int GetDimension() {return m_NumberOfHiddenNeurons[m_net.size()-1];};  // Override the Dimensionality Reduction model method, it is used in the dimensionality reduction filter to set the output image size
	itkGetMacro(NumberOfHiddenNeurons,itk::Array<unsigned int>);
	itkSetMacro(NumberOfHiddenNeurons,itk::Array<unsigned int>);

	itkGetMacro(NumberOfIterations,unsigned int);
	itkSetMacro(NumberOfIterations,unsigned int);

	itkGetMacro(Regularization,double);
	itkSetMacro(Regularization,double);

	itkGetMacro(Noise,double);
	itkSetMacro(Noise,double);

	itkGetMacro(rho,double);
	itkSetMacro(rho,double);

	itkGetMacro(beta,double);
	itkSetMacro(beta,double);

	bool CanReadFile(const std::string & filename);
	bool CanWriteFile(const std::string & filename);

	void Save(const std::string & filename, const std::string & name="")  ITK_OVERRIDE;
	void Load(const std::string & filename, const std::string & name="")  ITK_OVERRIDE;

	void Train() ITK_OVERRIDE;
	void TrainOneLayer(unsigned int, shark::Data<shark::RealVector> &);
	
protected:
	AutoencoderModel();	
	~AutoencoderModel() ITK_OVERRIDE;
 
	virtual TargetSampleType DoPredict(const InputSampleType& input) const ITK_OVERRIDE;
	virtual void DoPredictBatch(const InputListSampleType *, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType *) const ITK_OVERRIDE;
  
private:
	
	/** Network attributes */
	std::vector<AutoencoderType> m_net;
	
	
	itk::Array<unsigned int> m_NumberOfHiddenNeurons;
	/** Training parameters */
	unsigned int m_NumberOfIterations;
	double m_Regularization;  // L2 Regularization parameter
	double m_Noise;  // probability for an input to be set to 0 (denosing autoencoder)
	double m_rho; // Sparsity parameter
	double m_beta; // Sparsity regularization parameter
};
} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "AutoencoderModel.txx"
#endif


#endif

