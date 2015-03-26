// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_NODE
#define VOLPLAY_SDF_NODE

#include <volplay/types.h>
#include <volplay/sdf_result.h>
#include <unordered_map>
#include <string>

namespace volplay {

    /** A leaf node in the SDF (signed distance function) scene graph. */
    class SDFNode {
    public:
        /** Map for attachments */
        typedef std::unordered_map<std::string, SDFNodeAttachmentPtr> AttachmentMap;

        /** Evaluate the SDF at given position. Only returns the signed distance. */
        virtual Scalar eval(const Vector &x) const;
        
        /** Evaluate the SDF at the given position. Returns signed distance and additional information. */
        virtual SDFResult fullEval(const Vector &x) const = 0;
        
        /** Evaluate the gradient of the SDF at the given position.
         *  The gradient will always point in the direction of maximum distance increase.
         */
        virtual Vector gradient(const Vector &x, Scalar eps = Scalar(0.0001)) const;
        
        /** Calculate the approximate unit normal at the given position. */
        virtual Vector normal(const Vector &x, Scalar eps = Scalar(0.0001)) const;
        
        /** Sphere tracing options */
        struct TraceOptions {
            Scalar minT;
            Scalar maxT;
            Scalar stepFact;
            Scalar sdfThreshold;
            int maxIter;
            
            /** Default trace options */
            TraceOptions();
        };
        
        /** Contains advanced tracing result infos */
        struct TraceResult {
            int iter;               ///< Number of iterations applied
            Scalar t;               ///< Parametric t of ray equation
            Scalar sdf;             ///< Signed distance at intersection
            const SDFNode *node;    ///< Closed node
            bool hit;               ///< True if abs(sdf) < TraceOptions.sdfThreshold
            
            /** Default trace options */
            TraceResult();
        };

        /** Trace ray. Uses sphere tracing to find intersection */
        virtual Scalar trace(const Vector &o, const Vector &d, const TraceOptions &opts, TraceResult *tr = 0) const;

        /** Set attachments */
        void setAttachments(const AttachmentMap &other);
        
        /** Set node attachment */
        void setAttachment(const std::string &key, const SDFNodeAttachmentPtr &attachment);
        
        /** Get node attachment */
        template<class Derived>
        std::shared_ptr<Derived> attachment(const std::string &key) const
        {
            auto iter = _attachments.find(key);
            if (iter != _attachments.end()) {
                return std::dynamic_pointer_cast<Derived>(iter->second);
            } else {
                return std::shared_ptr<Derived>();
            }
        }

        /** Get node attachment or default */
        template<class Derived>
        std::shared_ptr<Derived> attachmentOrDefault(const std::string &key, const std::shared_ptr<Derived> &defaultValue) const
        {
            auto iter = _attachments.find(key);
            if (iter != _attachments.end()) {
                return std::dynamic_pointer_cast<Derived>(iter->second);
            } else {
                return defaultValue;                
            }
        }

        /** Get or create node attachment */
        template<class Derived>
        std::shared_ptr<Derived> getOrCreateAttachment(const std::string &key)
        {
            auto iter = _attachments.find(key);
            if (iter != _attachments.end()) {
                return std::dynamic_pointer_cast<Derived>(iter->second);
            } else {
                std::shared_ptr<Derived> d(new Derived());
                setAttachment(key, d);
                return d;
            }
        }

		/* Test if this node is able to group other nodes. */
		virtual bool isGroup() const;

		/* Accept a node visitor. Visitation starts at this node. */
		virtual void accept(SDFNodeVisitor &nv);

        /* Accept a node visitor. Visitation starts at child nodes. */
		virtual void acceptChildren(SDFNodeVisitor &nv);
        
    private:
        AttachmentMap _attachments;
    };

}

#endif
