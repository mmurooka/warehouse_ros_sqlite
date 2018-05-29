 /*
 * Copyright (c) 2008, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * \file 
 * 
 * Implementation of DummyResultIterator.
 *
 * \author Bhaskara Marthi
 */

#include <warehouse_ros_dummy/query_results.h>

namespace warehouse_ros_dummy
{

DummyResultIterator::DummyResultIterator(boost::shared_ptr<dummy::DBClientConnection> conn,
                                         const std::string& ns,
                                         const dummy::Query& query) :
  cursor_(new Cursor(conn->query(ns, query)))
{
  if ((*cursor_)->more())
    next_ = (*cursor_)->nextSafe();
}

bool DummyResultIterator::next()
{
  ROS_ASSERT (next_);
  if ((*cursor_)->more())\
  {
    next_ = (*cursor_)->nextSafe();
    return true;
  }
  else
  {
    next_.reset();
    return false;
  }
}

bool DummyResultIterator::hasData() const
{
  return (bool)next_;
}

warehouse_ros::Metadata::ConstPtr DummyResultIterator::metadata() const
{
  ROS_ASSERT(next_);
  //return typename warehouse_ros::Metadata::ConstPtr(new DummyMetadata(next_->copy())); // FIXME
  return typename warehouse_ros::Metadata::ConstPtr(new DummyMetadata(next_.get()));
}

std::string DummyResultIterator::message() const
{
  std::string id = (*next_)["_id"].OID().toString();
  int len;
  const char *buf = (*next_)[id].binData(len);
  return std::string(buf, buf+len);
}

bson::BSONObj DummyResultIterator::metadataRaw() const
{
  ROS_ASSERT(next_);
  //return next_->copy(); // FIXME
  return next_.get();
}

} // namespace
