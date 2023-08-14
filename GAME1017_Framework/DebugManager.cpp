#include "DebugManager.h"
#include "RenderManager.h"

void DebugManager::DrawLine(const SDL_Point start, const SDL_Point end, const SDL_Color col)
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), col.r, col.g, col.b, col.a);
	SDL_RenderDrawLine(REMA::GetRenderer(), start.x, start.y, end.x, end.y);
}

void DebugManager::DrawLine(const SDL_FPoint start, const SDL_FPoint end, const SDL_Color col)
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), col.r, col.g, col.b, col.a);
	SDL_RenderDrawLineF(REMA::GetRenderer(), start.x, start.y, end.x, end.y);
}

void DebugManager::DrawRect(const SDL_Rect rect, bool fill, const SDL_Color col)
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), col.r, col.g, col.b, col.a);
	if (fill = true)
		SDL_RenderFillRect(REMA::GetRenderer(), &rect);
	else
		SDL_RenderDrawRect(REMA::GetRenderer(), &rect);
}

void DebugManager::DrawRect(const SDL_FRect rect, bool fill, const SDL_Color col)
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), col.r, col.g, col.b, col.a);
	if (fill = true)
		SDL_RenderFillRectF(REMA::GetRenderer(), &rect);
	else
		SDL_RenderDrawRectF(REMA::GetRenderer(), &rect);
}

void DebugManager::DrawCircle(const SDL_FPoint centre, float radius, const SDL_Color col)
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), col.r, col.g, col.b, col.a);
	const float diameter = floor(radius * 2.0f);

	float x = (radius - 1);
	float y = 0;
	float tx = 1;
	float ty = 1;
	float error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPointF(REMA::GetRenderer(), centre.x + x, centre.y - y);
		SDL_RenderDrawPointF(REMA::GetRenderer(), centre.x + x, centre.y + y);
		SDL_RenderDrawPointF(REMA::GetRenderer(), centre.x - x, centre.y - y);
		SDL_RenderDrawPointF(REMA::GetRenderer(), centre.x - x, centre.y + y);
		SDL_RenderDrawPointF(REMA::GetRenderer(), centre.x + y, centre.y - x);
		SDL_RenderDrawPointF(REMA::GetRenderer(), centre.x + y, centre.y + x);
		SDL_RenderDrawPointF(REMA::GetRenderer(), centre.x - y, centre.y - x);
		SDL_RenderDrawPointF(REMA::GetRenderer(), centre.x - y, centre.y + x);
		
		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 255, 255, 255, 255);
}
