#include "_gen_geo_map.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <algorithm>
#include <vector>
#include <utility>
#include <iterator>
#include <stdio.h>
#include <math.h>

using std::vector; using cv::Mat; 

int dim2Idx(int i, int j, int W){
  return i * W + j;  
}


int dim3Idx(int i, int j, int k, int W, int C){
  return (i * W + j) * C + k;  
}


float interpPoint(float x1, float y1, float x2, float y2, float x){
  return (y2 - y1) / (x2 - x1 + 1e-6) * (x - x1) + y1;
}


void assignMatUsePtr(Mat & M, const float* data){
  float* pM = (float*)M.data;
  int num_elem = M.cols * M.rows * M.channels();
  std::copy_n(data, num_elem, pM);
}


void assignPtrUseMat(float* data, const Mat & M){
  const float* pM = (float*)M.data;
  int num_elem = M.cols * M.rows * M.channels();
  std::copy_n(pM, num_elem, data);
}


void fitReorderPoly(cv::Point2f * pts, float* gt_bboxes, int idx){
  float* ptmp = gt_bboxes + 8 * idx;
  vector<std::pair<float, float> > coord;  
  for(int j=0; j < 4; ++j){
    coord.push_back(std::make_pair(ptmp[2 * j], ptmp[2 * j + 1]));
  }
  std::sort(coord.begin(), coord.end(), [](std::pair<float, float> item1, 
                                           std::pair<float, float> item2){
                                          return item1.first < item2.first;});
  float x1 = coord[0].first; 
  float y1 = coord[0].second; 
  float x4 = coord[1].first; 
  float y4 = coord[1].second; 
  float x2 = coord[2].first; 
  float y2 = coord[2].second; 
  float x3 = coord[3].first; 
  float y3 = coord[3].second; 
  float xmin = x1 - 5, xmax = x3 + 5;

  if(y1 > y4){
    std::swap(y1, y4);
    std::swap(x1, x4);
  }

  if(y2 > y3){
    std::swap(y2, y3);
    std::swap(x2, x3);
  }

  // interp the points. 
  pts[0].x = xmin;
  pts[0].y = interpPoint(x1, y1, x2, y2, xmin);
  pts[1].x = xmax;
  pts[1].y = interpPoint(x1, y1, x2, y2, xmax);
  pts[2].x = xmax;
  pts[2].y = interpPoint(x3, y3, x4, y4, xmax);
  pts[3].x = xmin;
  pts[3].y = interpPoint(x3, y3, x4, y4, xmin);
}


void _genGeoMap(float* score_map, float* geo_map, float* gt_bboxes, float* gt_masks, 
                  int H, int W, int num_gt_bboxes){
  // copy the score_map to Mat.  
  Mat m_sc_map(H, W, CV_32FC1, cv::Scalar(0.f));
  Mat m_sc_mask(H, W, CV_32FC1, cv::Scalar(0.f));

  for (int idx=0; idx < num_gt_bboxes; ++idx){
    // clear the m_sc_mask
    m_sc_mask.setTo(cv::Scalar::all(0.f));
    cv::Point2f fit_points[1][4];
    cv::Point fit_points_i[1][4];
    fitReorderPoly(fit_points[0], gt_bboxes, idx);
    float x1 = fit_points[0][0].x;
    float y1 = fit_points[0][0].y;
    float x2 = fit_points[0][1].x;
    float y2 = fit_points[0][1].y;
    float x3 = fit_points[0][2].x;
    float y3 = fit_points[0][2].y;
    float x4 = fit_points[0][3].x;
    float y4 = fit_points[0][3].y;
    
    fit_points_i[0][0].x = fit_points[0][0].x;
    fit_points_i[0][0].y = fit_points[0][0].y;
    fit_points_i[0][1].x = fit_points[0][1].x;
    fit_points_i[0][1].y = fit_points[0][1].y;
    fit_points_i[0][2].x = fit_points[0][2].x;
    fit_points_i[0][2].y = fit_points[0][2].y;
    fit_points_i[0][3].x = fit_points[0][3].x;
    fit_points_i[0][3].y = fit_points[0][3].y;

    if(abs(y1 - y4) < 4. || abs(y2 - y3) < 4.)
      continue;

    if(abs(x1 - x2) < 4.)
      continue;

    int npt[] = {4};
    const cv::Point * ppt[1] = {fit_points_i[0]};
    
    if(gt_masks[idx] == 0.)
      cv::fillPoly(m_sc_mask, ppt, npt, 1, cv::Scalar(2.f));
    else
      cv::fillPoly(m_sc_mask, ppt, npt, 1, cv::Scalar(1.f));

    float shrink_ratio = 0.3;
    
    // set the geo_map. 
    for(int i = 0; i < m_sc_mask.rows; i++)
    {
      float* Mi = m_sc_mask.ptr<float>(i);
      for(int j = 0; j < m_sc_mask.cols; j++)
      {
        if(Mi[j] == 1.){
          float y_up = interpPoint(x1, y1, x2, y2, j);
          float y_down = interpPoint(x3, y3, x4, y4, j);
          // shrink 
          float up_off = i - y_up;
          float down_off = y_down - i;
          float hh = y_down - y_up + 1e-6;
          if((up_off / hh > shrink_ratio) && (down_off / hh > shrink_ratio)){
            geo_map[dim3Idx(i, j, 0, W, 2)] = y_up / H;
            geo_map[dim3Idx(i, j, 1, W, 2)] = y_down / H;
          }
          else
            Mi[j] = 2.;
        }
      }
    }
    m_sc_map = m_sc_map + m_sc_mask;
  }
  // set the score_map. 
  assignPtrUseMat(score_map, m_sc_map); 
}
