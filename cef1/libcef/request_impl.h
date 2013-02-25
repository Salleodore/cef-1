// Copyright (c) 2008-2009 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_LIBCEF_REQUEST_IMPL_H_
#define CEF_LIBCEF_REQUEST_IMPL_H_
#pragma once

#include "include/cef_request.h"
#include "third_party/WebKit/Source/Platform/chromium/public/WebHTTPBody.h"

namespace net {
class HttpRequestHeaders;
class UploadData;
class UploadDataStream;
class UploadElement;
class UploadElementReader;
class URLRequest;
};

namespace WebKit {
class WebURLRequest;
}

// Implementation of CefRequest
class CefRequestImpl : public CefRequest {
 public:
  CefRequestImpl();
  ~CefRequestImpl() {}

  virtual CefString GetURL() OVERRIDE;
  virtual void SetURL(const CefString& url) OVERRIDE;
  virtual CefString GetMethod() OVERRIDE;
  virtual void SetMethod(const CefString& method) OVERRIDE;
  virtual CefRefPtr<CefPostData> GetPostData() OVERRIDE;
  virtual void SetPostData(CefRefPtr<CefPostData> postData) OVERRIDE;
  virtual void GetHeaderMap(HeaderMap& headerMap) OVERRIDE;
  virtual void SetHeaderMap(const HeaderMap& headerMap) OVERRIDE;
  virtual void Set(const CefString& url,
                   const CefString& method,
                   CefRefPtr<CefPostData> postData,
                   const HeaderMap& headerMap) OVERRIDE;
  virtual RequestFlags GetFlags() OVERRIDE;
  virtual void SetFlags(RequestFlags flags) OVERRIDE;
  virtual CefString GetFirstPartyForCookies() OVERRIDE;
  virtual void SetFirstPartyForCookies(const CefString& url) OVERRIDE;

  void Set(net::URLRequest* request);
  void Set(const WebKit::WebURLRequest& request);
  void Get(WebKit::WebURLRequest& request);

  static void GetHeaderMap(const net::HttpRequestHeaders& headers,
                           HeaderMap& map);
  static void GetHeaderMap(const WebKit::WebURLRequest& request,
                           HeaderMap& map);
  static void SetHeaderMap(const HeaderMap& map,
                           WebKit::WebURLRequest& request);

 protected:
  CefString url_;
  CefString method_;
  CefRefPtr<CefPostData> postdata_;
  HeaderMap headermap_;

  // The below methods are used by WebURLRequest.
  RequestFlags flags_;
  CefString first_party_for_cookies_;

  IMPLEMENT_REFCOUNTING(CefRequestImpl);
  IMPLEMENT_LOCKING(CefRequestImpl);
};

// Implementation of CefPostData
class CefPostDataImpl : public CefPostData {
 public:
  CefPostDataImpl();
  ~CefPostDataImpl() {}

  virtual size_t GetElementCount() OVERRIDE;
  virtual void GetElements(ElementVector& elements) OVERRIDE;
  virtual bool RemoveElement(CefRefPtr<CefPostDataElement> element) OVERRIDE;
  virtual bool AddElement(CefRefPtr<CefPostDataElement> element) OVERRIDE;
  virtual void RemoveElements();

  void Set(const net::UploadData& data);
  void Set(const net::UploadDataStream& data_stream);
  void Get(net::UploadData& data);
  net::UploadDataStream* Get();
  void Set(const WebKit::WebHTTPBody& data);
  void Get(WebKit::WebHTTPBody& data);

 protected:
  ElementVector elements_;

  IMPLEMENT_REFCOUNTING(CefPostDataImpl);
  IMPLEMENT_LOCKING(CefPostDataImpl);
};

// Implementation of CefPostDataElement
class CefPostDataElementImpl : public CefPostDataElement {
 public:
  CefPostDataElementImpl();
  ~CefPostDataElementImpl();

  virtual void SetToEmpty() OVERRIDE;
  virtual void SetToFile(const CefString& fileName) OVERRIDE;
  virtual void SetToBytes(size_t size, const void* bytes) OVERRIDE;
  virtual Type GetType() OVERRIDE;
  virtual CefString GetFile() OVERRIDE;
  virtual size_t GetBytesCount() OVERRIDE;
  virtual size_t GetBytes(size_t size, void* bytes) OVERRIDE;

  void* GetBytes() { return data_.bytes.bytes; }

  void Set(const net::UploadElement& element);
  void Set(const net::UploadElementReader& element_reader);
  void Get(net::UploadElement& element);
  net::UploadElementReader* Get();
  void Set(const WebKit::WebHTTPBody::Element& element);
  void Get(WebKit::WebHTTPBody::Element& element);

 protected:
  Type type_;
  union {
    struct {
      void* bytes;
      size_t size;
    } bytes;
    cef_string_t filename;
  } data_;

  IMPLEMENT_REFCOUNTING(CefPostDataElementImpl);
  IMPLEMENT_LOCKING(CefPostDataElementImpl);
};

#endif  // CEF_LIBCEF_REQUEST_IMPL_H_
