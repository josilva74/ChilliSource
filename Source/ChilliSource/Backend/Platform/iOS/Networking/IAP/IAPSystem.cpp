//
//  IAPSystem.cpp
//  MoFlow
//
//  Created by Scott Downie on 12/06/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Networking/IAP/IAPSystem.h>

#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>

namespace ChilliSource
{
    namespace iOS
    {
        CIAPSystem::CIAPSystem()
        {
            mpStoreKitSystem = [[StoreKitIAPSystem alloc] init];
        }
        //---------------------------------------------------------------
        /// Register Products
        //---------------------------------------------------------------
        void CIAPSystem::RegisterProducts(const std::vector<Networking::IAPProductRegInfo>& inaProducts)
        {
            mProductRegInfos = inaProducts;
        }
        //---------------------------------------------------------------
        /// Get Provider ID
        //---------------------------------------------------------------
        std::string CIAPSystem::GetProviderID() const
        {
            return "Apple";
        }
        //---------------------------------------------------------------
        /// Get Provider Name
        //---------------------------------------------------------------
        std::string CIAPSystem::GetProviderName() const
        {
            return "iTunes";
        }
        //---------------------------------------------------------------
        /// Is Purchasing Enabled
        //---------------------------------------------------------------
        bool CIAPSystem::IsPurchasingEnabled()
        {
            return [mpStoreKitSystem isPurchasingEnabled] == YES;
        }
        //---------------------------------------------------------------
        /// Start Listening For Transaction Updates
        //---------------------------------------------------------------
        void CIAPSystem::StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate)
        {
            mTransactionDelegate = inRequestDelegate;
            
            if(mTransactionDelegate == nullptr)
                return;
            
            [mpStoreKitSystem startListeningForTransactions:fastdelegate::MakeDelegate(this, &CIAPSystem::OnTransactionUpdate)];
        }
        //---------------------------------------------------------------
        /// On Transaction Update
        //---------------------------------------------------------------
        void CIAPSystem::OnTransactionUpdate(NSString* inProductID, StoreKitIAP::TransactionResult ineResult, SKPaymentTransaction* inpTransaction)
        {
            if(mTransactionDelegate == nullptr)
                return;
            
            Networking::IAPTransactionPtr pTransaction(new Networking::IAPTransaction());
            pTransaction->strProductID = Core::CStringUtils::NSStringToString(inProductID);
            
            bool bHasReceipt = false;
            Networking::IAPTransaction::Status eResult = Networking::IAPTransaction::Status::k_failed;
            switch(ineResult)
            {
                case StoreKitIAP::TransactionResult::k_succeeded:
                    bHasReceipt = true;
                    eResult = Networking::IAPTransaction::Status::k_succeeded;
                    break;
                case StoreKitIAP::TransactionResult::k_failed:
                    eResult = Networking::IAPTransaction::Status::k_failed;
                    break;
                case StoreKitIAP::TransactionResult::k_cancelled:
                    eResult = Networking::IAPTransaction::Status::k_cancelled;
                    break;
                case StoreKitIAP::TransactionResult::k_restored:
                    bHasReceipt = true;
                    eResult = Networking::IAPTransaction::Status::k_restored;
                    break;
                case StoreKitIAP::TransactionResult::k_resumed:
                    bHasReceipt = true;
                    eResult = Networking::IAPTransaction::Status::k_resumed;
                    break;
            }
            
            if(pTransaction)
            {
                pTransaction->strTransactionID = Core::CStringUtils::NSStringToString(inpTransaction.transactionIdentifier);
                
                if(bHasReceipt)
                {
                    pTransaction->strReceipt = Core::CBaseEncoding::Base64Encode((s8*)[inpTransaction.transactionReceipt bytes], [inpTransaction.transactionReceipt length]);
                }
            }
            
            mTransactionDelegate(eResult, pTransaction);
        }
        //---------------------------------------------------------------
        /// Stop Listening For Transaction Updates
        //---------------------------------------------------------------
        void CIAPSystem::StopListeningForTransactionUpdates()
        {
            [mpStoreKitSystem stopListeningForTransactions];
        }
        //---------------------------------------------------------------
        /// Request Product Descriptions
        //---------------------------------------------------------------
        void CIAPSystem::RequestProductDescriptions(const std::vector<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
            mProductDescDelegate = inRequestDelegate;
            
            if(mProductDescDelegate == nullptr)
                return;
            
            if(inaProductIDs.empty())
            {
                mProductDescDelegate(std::vector<Networking::IAPProductDesc>());
                return;
            }
            
            NSMutableSet* idSet = [[NSMutableSet alloc] initWithCapacity:inaProductIDs.size()];
			
			for (u32 nID = 0; nID < inaProductIDs.size(); nID++)
            {
				[idSet addObject:Core::CStringUtils::StringToNSString(inaProductIDs[nID])];
			}
			
            [mpStoreKitSystem requestProducts:idSet forDelegate:fastdelegate::MakeDelegate(this, &CIAPSystem::OnProductDescriptionRequestComplete)];
            
            [idSet release];
        }
        //---------------------------------------------------------------
        /// Request All Product Descriptions
        //---------------------------------------------------------------
        void CIAPSystem::RequestAllProductDescriptions(const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
            std::vector<std::string> aIDs;
            aIDs.reserve(mProductRegInfos.size());
            
            for(u32 i=0; i<mProductRegInfos.size(); ++i)
            {
                aIDs.push_back(mProductRegInfos[i].strID);
            }
            
            RequestProductDescriptions(aIDs, inRequestDelegate);
        }
        //---------------------------------------------------------------
        /// On Product Description Request Complete
        //---------------------------------------------------------------
        void CIAPSystem::OnProductDescriptionRequestComplete(NSArray* inProducts)
        {
            if(mProductDescDelegate == nullptr)
                return;
 
            std::vector<Networking::IAPProductDesc> aResults;
            
            if(inProducts != nil)
            {
                NSNumberFormatter *pFormatter = [[NSNumberFormatter alloc] init];
                [pFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
                [pFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                
                for(SKProduct* pProduct in inProducts)
                {
                    Networking::IAPProductDesc sDesc;
                    sDesc.strID = Core::CStringUtils::NSStringToString(pProduct.productIdentifier);
                    sDesc.strName = Core::CStringUtils::NSStringToString(pProduct.localizedTitle);
                    sDesc.strDescription = Core::CStringUtils::NSStringToString(pProduct.localizedDescription);
                    
                    [pFormatter setLocale:pProduct.priceLocale];
                    sDesc.strFormattedPrice = Core::CStringUtils::NSStringToString([pFormatter stringFromNumber:pProduct.price]);
                    
                    NSLocale* storeLocale = pProduct.priceLocale;
                    sDesc.strCountryCode = Core::CStringUtils::NSStringToString((NSString*)CFLocaleGetValue((CFLocaleRef)storeLocale, kCFLocaleCountryCode));
                    
                    aResults.push_back(sDesc);
                }
                
                [pFormatter release];
            }
            

            mProductDescDelegate(aResults);
        }
        //---------------------------------------------------------------
        /// Cancel Product Descriptions Request
        //---------------------------------------------------------------
        void CIAPSystem::CancelProductDescriptionsRequest()
        {
            [mpStoreKitSystem cancelProductsRequest];
        }
        //---------------------------------------------------------------
        /// Is Product ID Registered
        //---------------------------------------------------------------
        bool IsProductIDRegistered(const std::vector<Networking::IAPProductRegInfo>& inProductRegInfos, const std::string& instrProductID)
        {
            for(u32 i=0; i<inProductRegInfos.size(); ++i)
            {
                if(inProductRegInfos[i].strID == instrProductID)
                {
                    return true;
                }
            }
            
            return false;
        }
        //---------------------------------------------------------------
        /// Request Product Purchase
        //---------------------------------------------------------------
        void CIAPSystem::RequestProductPurchase(const std::string& instrProductID)
        {
            MOFLOW_ASSERT(IsProductIDRegistered(mProductRegInfos, instrProductID), "Products must be registered with the IAP system before purchasing");
            NSString* productID = Core::CStringUtils::StringToNSString(instrProductID);
            [mpStoreKitSystem requestPurchaseWithProductID:productID andQuantity:1];
        }
        //---------------------------------------------------------------
        /// Close Transaction
        //---------------------------------------------------------------
        void CIAPSystem::CloseTransaction(const Networking::IAPTransactionPtr& inpTransaction, const Networking::IAPTransactionCloseDelegate& inDelegate)
        {
            [mpStoreKitSystem closeTransactionWithID:Core::CStringUtils::StringToNSString(inpTransaction->strTransactionID)];
            
            if(inDelegate)
            {
                inDelegate(inpTransaction->strProductID, inpTransaction->strTransactionID);
            }
        }
        //---------------------------------------------------------------
        /// Restore Non Consumable Purchases
        //---------------------------------------------------------------
        void CIAPSystem::RestoreManagedPurchases()
        {
            [mpStoreKitSystem restoreNonConsumablePurchases];
        }
        CIAPSystem::~CIAPSystem()
        {
            [mpStoreKitSystem release];
        }
    }
}