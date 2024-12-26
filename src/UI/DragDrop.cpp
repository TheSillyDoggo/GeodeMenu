#include "DragDrop.hpp"

#ifdef GEODE_IS_WINDOWS

// A much simpler way than whatever shit i was doing for that other winapi mod
HWND getWindowHandle()
{
    return WindowFromDC(wglGetCurrentDC());
}

class DropTarget : IDropTarget
{
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject){ return S_OK; }

	virtual ULONG STDMETHODCALLTYPE AddRef(void){ return 0; }

	virtual ULONG STDMETHODCALLTYPE Release(void){ return 0; }

	virtual HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
	{
		if(pdwEffect == NULL)
			return E_INVALIDARG;

		*pdwEffect |= DROPEFFECT_COPY;

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
	{
		if(pdwEffect == NULL)
			return E_INVALIDARG;

		*pdwEffect |= DROPEFFECT_COPY;

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE DragLeave(void)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
	{
		if(pdwEffect == NULL)
			return E_INVALIDARG;

		*pdwEffect |= DROPEFFECT_COPY;

		FORMATETC format;
		STGMEDIUM medium;
		format.cfFormat = CF_HDROP;
		format.ptd = NULL;
		format.dwAspect = DVASPECT_CONTENT;
		format.lindex = -1;
		format.tymed = TYMED_HGLOBAL;
		medium.tymed = TYMED_HGLOBAL;
		HRESULT res = pDataObj->GetData(&format, &medium);
		HDROP drop = (HDROP)medium.hGlobal;
		wchar_t* fileName = NULL;
		//See https://docs.microsoft.com/en-us/windows/desktop/api/shellapi/nf-shellapi-dragqueryfilew
		UINT filePathesCount = DragQueryFile(drop, 0xFFFFFFFF, NULL, 512);//If "0xFFFFFFFF" as the second parameter: return the count of files dropped
		UINT longestFileNameLength = 0;

		std::vector<std::string> paths;

		for(UINT i = 0; i < filePathesCount; ++i)
		{
			//If NULL as the third parameter: return the length of the path, not counting the trailing '0'
			UINT fileNameLength = DragQueryFile(drop, i, NULL, 512) + 1;
			if(fileNameLength > longestFileNameLength)
			{
				if(fileName != NULL)
				{
					free(fileName);
				}
				longestFileNameLength = fileNameLength;
				fileName = (wchar_t*)malloc(longestFileNameLength * sizeof(*fileName));
			}
			DragQueryFileW(drop, i, fileName, fileNameLength);

			auto wstr = std::wstring(fileName);
			auto str = std::string(wstr.begin(), wstr.end());

			paths.push_back(str);
		}
		if(fileName != NULL)
		{
			free(fileName);
		}
		ReleaseStgMedium(&medium);

		DragDrop::get()->invoke(paths);

		return S_OK;
	}
};

DropTarget* target;

void init()
{
	OleInitialize(NULL);

    target = new DropTarget();

    RegisterDragDrop(getWindowHandle(), (IDropTarget*)(target));
}

DragDrop::DragDrop()
{
	if (CCApplication::get())
	{
		init();
	}
	else
	{
		Loader::get()->queueInMainThread([this]
		{
			init();
		});
	}
}

#else

DragDrop::DragDrop()
{

}

#endif

DragDrop* DragDrop::get()
{
    static DragDrop* instance = nullptr;

    if (!instance)
        instance = new DragDrop();

    return instance;
}

void DragDrop::addListener(std::string id, DragDropCallback callback)
{
    callbacks.emplace(id, callback);
}

void DragDrop::removeListener(std::string id)
{
    if (callbacks.contains(id))
        callbacks.erase(id);
}

void DragDrop::invoke(std::vector<std::string> paths)
{
	for (auto callback : callbacks)
	{
		callback.second(paths);
	}
}