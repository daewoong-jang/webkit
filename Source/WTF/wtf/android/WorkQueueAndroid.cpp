/*
    Copyright (C) 2012 Samsung Electronics
    Copyright (C) 2015 NAVER Corp.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "WorkQueue.h"

#include "DispatchQueue.h"
#include "DispatchQueueWorkItem.h"

namespace WTF {

void WorkQueue::platformInitialize(const char* name, Type, QOS)
{
    m_dispatchQueue = DispatchQueue::create(name);
}

void WorkQueue::platformInvalidate()
{
    RefPtr<DispatchQueue> dispatchQueue = m_dispatchQueue.copyRef();
    dispatchQueue->stopThread();
}

void WorkQueue::registerSocketEventHandler(int fileDescriptor, std::function<void ()> function)
{
    if (!m_dispatchQueue)
        return;

    m_dispatchQueue->setSocketEventHandler(fileDescriptor, function);
}

void WorkQueue::unregisterSocketEventHandler(int fileDescriptor)
{
    UNUSED_PARAM(fileDescriptor);

    if (!m_dispatchQueue)
        return;

    m_dispatchQueue->clearSocketEventHandler();
}

class WorkQueueItemContext : public WorkItemContext {
public:
    WorkQueueItemContext(RefPtr<WorkQueue>&& workQueue)
        : m_workQueue(WTFMove(workQueue))
    { }

private:
    RefPtr<WorkQueue> m_workQueue;
};

void WorkQueue::dispatch(Function<void ()>&& function)
{
    if (!m_dispatchQueue)
        return;

    m_dispatchQueue->dispatch(WorkItem::create(WorkQueueItemContext(this), WTFMove(function)));
}

void WorkQueue::dispatchAfter(Seconds duration, Function<void ()>&& function)
{
    if (!m_dispatchQueue)
        return;

    m_dispatchQueue->dispatch(TimerWorkItem::create(WorkQueueItemContext(this), WTFMove(function), duration));
}

} // namespace WTF
