/*
 * =====================================================================================
 *
 *       Filename: transponder.hpp
 *        Created: 04/23/2016 10:51:19
 *  Last Modified: 06/18/2016 23:37:47
 *
 *    Description: base of actor model in mir2x, Theron::Actor acitvated at create
 *                 time so no way to control it, instead Transponder can 
 *                      1. react to message by callback
 *                      2. activate when needed
 *                      3. delete it to disable it
 *
 *                 it's not an object necessarily, in monoserver, an ``object" means
 *                 it has (UID(), AddTime()), but an transponder may not consist of
 *                 these attributes.
 *
 *                 Transponder : with an actor pod, override Operate() for operation
 *                 ReactObject : is an ``object" which has an acotr pod, and override
 *                               Operate() for operation
 *                 ActiveObject: is an ReactObject
 *
 *                 ReactObject is not an transponder, it's an ServerObject, because
 *                 I am trying to avoid MI.
 *
 *
 *                 Hook can be install/uninstall before / after Activate(), good
 *                 
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#pragma once
#include <queue>
#include <vector>
#include <utility>
#include <functional>

#include <Theron/Theron.h>

#include "statehook.hpp"
#include "delaycmd.hpp"
#include "messagepack.hpp"

class ActorPod;
class Transponder
{
    protected:
        ActorPod    *m_ActorPod;

        // the document of Theron says we can call GetAddress() outside of 
        // Actor, but is that safe?
        Theron::Address m_ThisAddress;


    protected:
        // TODO & TBD
        // use trigger here since most of the time we are traversing
        // rather than install/uninstall trigger
        StateHook m_StateHook;
        std::priority_queue<DelayCmd> m_DelayCmdQ;

    public:
        Transponder();
        virtual ~Transponder();

    public:
        bool ActorPodValid();
        bool AccessCheck();
        
    public:
        virtual void Operate(const MessagePack &, const Theron::Address &) = 0;

    public:
        virtual Theron::Address Activate();

        Theron::Address GetAddress()
        {
            return m_ThisAddress;
        }

    public:
        void Delay(uint32_t, const std::function<void()> &);

#if defined(MIR2X_DEBUG) && (MIR2X_DEBUG >= 5)
    protected:
        virtual const char *ClassName() = 0;
#endif
};
