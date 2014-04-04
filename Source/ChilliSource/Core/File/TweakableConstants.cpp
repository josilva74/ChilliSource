//
//  TweakableConstants.cpp
//  Chilli Source
//
//  Created by Stuart McGaw
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/File/TweakableConstants.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/XML/tinyxml.h>

#include <climits>
#include <cfloat>
#include <algorithm>

namespace ChilliSource
{
	namespace Core
	{
        f32 SamplePointCurve(const TweakableConstants::CurvePoint * inpPointsArray, u32 inudwCount, f32 infXValue);
		
		f32 SamplePointCurve(const TweakableConstants::CurvePoint * inpPointsArray, u32 inudwCount, f32 infXValue){
			
			u32 udwLowerIndex = 0;
			u32 udwUpperIndex = 0;
			
			while (udwUpperIndex < (inudwCount-1) && inpPointsArray[udwUpperIndex].X < infXValue) {
				udwUpperIndex++;
				udwLowerIndex = udwUpperIndex - 1;
			}
			
			if (udwUpperIndex == 0) {
				return inpPointsArray[0].Y;
			} else {
				
				f32 fLowerXValue = inpPointsArray[udwLowerIndex].X;
				f32 fLowerYValue = inpPointsArray[udwLowerIndex].Y;
				f32 fUpperXValue = inpPointsArray[udwUpperIndex].X;
				f32 fUpperYValue = inpPointsArray[udwUpperIndex].Y;
				
				f32 fXProportion = (infXValue - fLowerXValue)/(fUpperXValue - fLowerXValue);
				fXProportion = std::min(fXProportion,1.0f);
				
				return fLowerYValue + fXProportion * (fUpperYValue - fLowerYValue);
			}
			
			return FLT_MAX;
		}
		
		TweakableConstants::TweakableConstants()
		{
			LoadValues();
			msSingleton = this;
		}
		TweakableConstants::~TweakableConstants(){
			
		}
		TweakableConstants &   TweakableConstants::GetSingleton(){
			return *msSingleton;
		}
		TweakableConstants *   TweakableConstants::GetSingletonPtr(){
			return msSingleton;
		}
		void TweakableConstants::AddSourceFile(StorageLocation ineStorageLocation, const std::string & fileName, bool loadNow)
        {
			bool bAlreadyExists = false;
            
            //Quick test for identical filenames.
			//TODO Need to resolve paths properly using Windows stuff to ensure no repeated definitions
            for (std::vector<StorageLocationAndFilenamePtr>::iterator it = mFileNames.begin(); it != mFileNames.end(); ++it)
            {
                if ((*it)->mstrFilename == fileName && (*it)->meStorageLocation == ineStorageLocation)
                {
                    bAlreadyExists = true;
                    break;
                }
            }
            
			if (bAlreadyExists == false){
                StorageLocationAndFilenamePtr SLAndFile(new StorageLocationAndFilename());
                SLAndFile->mstrFilename = fileName;
                SLAndFile->meStorageLocation = ineStorageLocation;
				mFileNames.push_back(SLAndFile);
				
				if (loadNow){
					LoadFile(ineStorageLocation, fileName);
				}
				
			}
			
		}
		void TweakableConstants::Reload(){
			LoadValues();
		}
		
		float TweakableConstants::GetFloat(const std::string & inNameSpace,const std::string & inConstName) const{
			MapNamespaceToConstantsGroupCItr pConstantsGroup = mNamespaces.find(inNameSpace);
			
			if (pConstantsGroup != mNamespaces.end()){
				MapNameToFloatItr pConstant = pConstantsGroup->second.Floats.find(inConstName);
				if (pConstant != pConstantsGroup->second.Floats.end()){
					return pConstant->second;
				}
			}
			
			CS_LOG_WARNING("Request for undefined TweakConstant float in namespace: " + inNameSpace + " with name: " + inConstName);
			
			return FLT_MAX;
		}
		float TweakableConstants::GetFloat(const std::string & constName) const{
			return GetFloat(mBlank,constName);
		}
		
		int TweakableConstants::GetInt(const std::string & inNameSpace, const std::string & inConstName) const{
			MapNamespaceToConstantsGroupCItr pConstantsGroup = mNamespaces.find(inNameSpace);
			
			if (pConstantsGroup != mNamespaces.end()){
				MapNameToIntItr pConstant = pConstantsGroup->second.Ints.find(inConstName);
				if (pConstant != pConstantsGroup->second.Ints.end()){
					return pConstant->second;
				}
			}
			
			CS_LOG_WARNING("Request for undefined TweakConstant int in namespace: " + inNameSpace + " with name: " + inConstName);
			return INT_MAX;
		}
		int TweakableConstants::GetInt(const std::string & constName) const{
			return GetInt(mBlank,constName);
		}
		
		const std::string & TweakableConstants::GetString(const std::string & inNameSpace, const std::string & inConstName) const{
			MapNamespaceToConstantsGroupCItr pConstantsGroup = mNamespaces.find(inNameSpace);
			
			if (pConstantsGroup != mNamespaces.end()){
				MapNameToStringItr pConstant = pConstantsGroup->second.Strings.find(inConstName);
				if (pConstant != pConstantsGroup->second.Strings.end()){
					return pConstant->second;
				}
			}
			
			CS_LOG_WARNING("Request for undefined TweakConstant string in namespace: " + inNameSpace + " with name: " + inConstName);
			return mBlank;
		}
		const std::string & TweakableConstants::GetString(const std::string & constName) const{
			return GetString(mBlank,constName);
		}
		
		bool TweakableConstants::GetBool(const std::string & inNameSpace, const std::string & inConstName) const{
			
			MapNamespaceToConstantsGroupCItr pConstantsGroup = mNamespaces.find(inNameSpace);
			
			if (pConstantsGroup != mNamespaces.end()){
				MapNameToBoolItr pConstant = pConstantsGroup->second.Bools.find(inConstName);
				if (pConstant != pConstantsGroup->second.Bools.end()){
					return pConstant->second;
				}
			}
			
			CS_LOG_WARNING("Request for undefined TweakConstant boolean in namespace: " + inNameSpace + " with name: " + inConstName);
			return false;
		} 
		bool TweakableConstants::GetBool(const std::string & constName) const{
			return GetBool(mBlank,constName);
		}
		f32 TweakableConstants::SampleCurve(const std::string & inNameSpace, const std::string & inCurveName, f32 infXValue) const{
			
			MapNamespaceToConstantsGroupCItr pConstantsGroup = mNamespaces.find(inNameSpace);
			
			if (pConstantsGroup != mNamespaces.end()){
				MapNameToCurvePointsItr pConstant = pConstantsGroup->second.Curves.find(inCurveName);
				if (pConstant != pConstantsGroup->second.Curves.end()){
					return SamplePointCurve(&pConstant->second[0], pConstant->second.size(), infXValue);
				}
			}
			
			CS_LOG_WARNING("Request for undefined TweakConstant curve in namespace: " + inNameSpace + " with name: " + inCurveName);
			return FLT_MAX;
			
		}
		f32 TweakableConstants::SampleCurve(const std::string & inCurveName, f32 infXValue) const{
			return SampleCurve(mBlank,inCurveName,infXValue);
		}
		void TweakableConstants::LoadValues(){
			
			mNamespaces.clear();
			
			//Create default namespace
			mNamespaces[mBlank] = ConstantsGroup();
			
			for (u32 nFile = 0; nFile < mFileNames.size(); nFile++){
				LoadFile(mFileNames[nFile]->meStorageLocation, mFileNames[nFile]->mstrFilename);
			}
			
			
		}
		
		void TweakableConstants::LoadFile(StorageLocation ineStorageLocation, const std::string & fileName){
			TiXmlDocument Document;
			Document.LoadFile(ineStorageLocation, fileName);
			
			TiXmlElement * pRoot = Document.RootElement();
			
			if (pRoot != nullptr && pRoot->ValueStr() == "TweakableConstants"){
				
				TiXmlElement * pElement = pRoot->FirstChildElement();
				
				while (pElement != nullptr){
					
					ParseFileElement(pElement, mBlank);
					pElement = pElement->NextSiblingElement();
				}
			}
		}
		
		void TweakableConstants::ParseFileElement(TiXmlElement * inpElement, const std::string & inCurrentNamespace){
			const char * pName = inpElement->Attribute("name");
			const char * pValue = inpElement->Attribute("value");
			
			if (pName == nullptr)
				return;
			
			if (inCurrentNamespace == mBlank){
				
				if (inpElement->ValueStr() == "Namespace"){
					std::string NamespaceName = std::string(pName);
					
					if (mNamespaces.find(NamespaceName) == mNamespaces.end()){
						mNamespaces[NamespaceName] = ConstantsGroup();
					}
					
					TiXmlElement * pSubElement = inpElement->FirstChildElement();
					
					while (pSubElement != nullptr){
						ParseFileElement(pSubElement, NamespaceName);
						pSubElement = pSubElement->NextSiblingElement();
					}
					return;
				}
			}
			if (pValue != nullptr){
				
				if (inpElement->ValueStr() == "Float"){  
					mNamespaces[inCurrentNamespace].Floats[pName] =  ParseF32(std::string(pValue));
				} else if (inpElement->ValueStr() == "Int"){
					mNamespaces[inCurrentNamespace].Ints[pName] =  ParseS32(std::string(pValue));
				} else if (inpElement->ValueStr() == "String"){
					mNamespaces[inCurrentNamespace].Strings[pName] = std::string(pValue);
				} else if (inpElement->ValueStr() == "Bool"){
					mNamespaces[inCurrentNamespace].Bools[pName] = ParseBool(std::string(pValue));
				} 
					

			} else if (inpElement->ValueStr() == "Curve") {
				
				CurvePointVector aPoints;
				
				TiXmlElement * pPointDef = inpElement->FirstChildElement();
				
				while (pPointDef != nullptr) {
					
					f64 X;
					f64 Y;
					pPointDef->Attribute("x", &X);
					pPointDef->Attribute("y", &Y);
					CurvePoint sPoint;
					sPoint.X = (f32) X;
					sPoint.Y = (f32) Y;
					
					aPoints.push_back(sPoint);
					
					pPointDef = pPointDef->NextSiblingElement();
				}
				mNamespaces[inCurrentNamespace].Curves[pName] = aPoints;
			}
		}
		
		TweakableConstants * TweakableConstants::msSingleton = 0;
	}
	
}