/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2020, PardCode

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
//
//Special thanks to albinopapa https://github.com/albinopapa for contributing to improve this code

#include "Window.h"
#include <stdexcept>

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
Window::Window()
{

}

bool Window::init()
{	
	//Setting up WNDCLASSEX object
	WNDCLASSEX wc{}; // Using {} to initialize a struct sets all fields to 0 in C++
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.hbrBackground = ( HBRUSH )COLOR_WINDOW;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	wc.hInstance = GetModuleHandle( nullptr );
	wc.lpszClassName = "MyWindowClass";
	wc.lpfnWndProc = &WndProc;

	// If the registration of class will fail, the function will return false
	if( !RegisterClassEx( &wc ) )
		throw std::runtime_error( "Filed to register WNDCLASSEX." );

	// When you create a window the dimensions include the non-client area such as the 
	// border and title bar, I'd use the AdjustWindowRectEx API
	RECT rect = { 0,0,1024,768 };
	AdjustWindowRectEx( &rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW );

	const auto width = rect.right - rect.left;
	const auto height = rect.bottom - rect.top;

	//Creation of the window
	m_hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		"MyWindowClass",
		"DirectX Application",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		nullptr,
		this );

	//if the creation fail return false
	if( !m_hwnd )
		throw std::runtime_error( "Filed to initialize Window." );

	//show up the window
	ShowWindow( m_hwnd, SW_SHOW );
	return true;
}


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch( msg )
	{
		case WM_CREATE:
		{
			// Event fired when the window is created
			// collected here..
			auto* const params = reinterpret_cast< LPCREATESTRUCT >( lparam );
			auto* const window =
				reinterpret_cast< Window* const >( params->lpCreateParams );

			// .. and then stored for later lookup
			SetWindowLongPtr( hwnd, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( window ) );
			window->onCreate();
			break;
		}

		case WM_DESTROY:
		{
			// Event fired when the window is destroyed
			auto* const window =
				reinterpret_cast< Window* const >( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
			window->onDestroy();
			PostQuitMessage( 0 );
			break;
		}


		default:
			return DefWindowProc( hwnd, msg, wparam, lparam );
	}

	return 0;
}

void Window::broadcast()
{
	MSG msg{};
	while( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) > 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	onUpdate();

	Sleep( 1 );
}

bool Window::isRun()
{
	return m_is_run;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}

Window::~Window()
{
	DestroyWindow( m_hwnd );
}
