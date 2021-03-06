/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef SIMULATOR_REQUEST_GENERATION_H_
#define SIMULATOR_REQUEST_GENERATION_H_

#include "simulator_client_types.h"

/**
 * @class RequestGeneration
 */
class RequestGeneration
{
    public:
        typedef std::function<void (int, OperationState)> ProgressStateCallback;

        /**
         * This method is to start the generation of the given type request
         * @param[in] type     request type
         * @param[in] id       request id
         * @param[in] callback callback function
         */
        RequestGeneration(RequestType type, int id,
                          ProgressStateCallback callback);
        /**
         * get the type of request
         * @return request type
         */
        RequestType type() const
        {
            return m_type;
        }
        /**
         * get the request id
         * @return request id
         */
        int id() const
        {
            return m_id;
        }
        /** This method is to start the request */
        void start();
        /** This method is to stop the request */
        void stop();

    protected:
        virtual void startSending() = 0;
        virtual void stopSending() = 0;

        RequestType m_type;
        int m_id;
        ProgressStateCallback m_callback;
        bool m_requestsSent;
        int m_requestCnt;
        int m_responseCnt;
};

#endif
