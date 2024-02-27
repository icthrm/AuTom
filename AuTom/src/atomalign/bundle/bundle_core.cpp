#include "bundle_core.h"
#include "params.h"
#include "compiler.h"
#include <math.h>
#include "dataf/calibration.h"
#include <vector>
#include "matrix/matrix.h"
#include "micros.h"
#include "util/exception.h"
#include "triangulate.h"
#include <unistd.h>
#include <fcntl.h>

#ifndef ABS
#define ABS(x) ( ( (x) < 0 )? -(x) : (x) )
#endif

using namespace bundle;

void BundleApp::InitGeometryData(util::TrackSpace& trackspace)
{
    std::vector<ImageData>& image_data = data.m_image_data;
    std::vector<TrackData>& track_data = data.m_track_data;
    MatchTable& matches = data.m_matches;

    image_data.clear();
    for(int i = 0; i < trackspace.Size(); i++){
        ImageData data;
        data.m_angle_in_mrc = trackspace.Z_Angle(i);
        image_data.push_back(data);
    }
    // Create the match table
    matches = MatchTable(trackspace.Size());

    for(int i = 0; i < trackspace.Size(); i++){
        util::TrackSpace::Iterator itr = trackspace.Z_Iterator(i);
        int num = 0;
        while(!itr.IsNULL()){
            image_data[i].AddKey(itr.X(), itr.Y());
            itr.SetExtraAsZVectorIndex(num);
            num++;
            itr++;
        }
    }

    for(int i = 0; i < trackspace.Size(); i++){
        util::TrackSpace::Iterator itr = trackspace.Z_Iterator(i);

        while(!itr.IsNULL()){
            TrackData new_track;

            util::TrackSpace::TrackNode node_itr = util::TrackSpace::TrackNode(itr);

            if(node_itr.IsBegin()){
                std::vector<ImageKey>& key_v = new_track.m_views;

                for(; !node_itr.IsNULL(); node_itr++){
                    key_v.push_back(ImageKey(node_itr.Z(), (int)node_itr.VecIndex()));
                }

                if(key_v.size() >=2){
                    track_data.push_back(new_track);
                }
// 		EX_TRACE("Point with %d projections found\n", (int)key_v.size());

                key_v.clear();
            }

            itr++;
        }
    }

    for(int i = 0; i < track_data.size(); i++){
        int num_features = (int) track_data[i].m_views.size();

        for (int j = 0; j < num_features; j++){
            int img1 = track_data[i].m_views[j].first;
            int key1 = track_data[i].m_views[j].second;

            image_data[img1].m_visible_points.push_back(i);
            image_data[img1].m_visible_keys.push_back(key1);
            image_data[img1].m_keys[key1].m_track = i;
        }
    }

    /* Set match flags and full fill the matchtable*/
    for(int i = 0; i < track_data.size(); i++){
        TrackData &track = track_data[i];
        int num_views = (int) track.m_views.size();

        for (int j = 0; j < num_views; j++){
            int img1 = track.m_views[j].first;
            int k1 = track.m_views[j].second;

            assert(img1 >= 0 && img1 < image_data.size());

            for (int k = j+1; k < num_views; k++){
                int img2 = track.m_views[k].first;
                int k2 = track.m_views[k].second;

                assert(img2 >= 0 && img2 < image_data.size());

                matches.SetMatch(MatchIndex(img1, img2));
                matches.GetMatchList(MatchIndex(img1, img2)).push_back(KeypointMatch(k1, k2));

                matches.SetMatch(MatchIndex(img2, img1));
                matches.GetMatchList(MatchIndex(img2, img1)).push_back(KeypointMatch(k2, k1));
            }
        }
    }

    EX_NOTE(PrintTrackBin();)

    trackspace.Release();
}

void BundleApp::InitBoundary(int _width, int _height)
{
    width = _width;
    height = _height;
}

/* Pick a good initial pair of cameras to bootstrap the bundle adjustment */
int BundleApp::PickInitialPair(int* i_best, int* j_best)
{
#define MATCH_THRESHOLD 32

    int num_images = data.NumImages();
	*i_best = -1;
    for(int i = 0; i < num_images; i++){
        if(data.OriAngles(i) <= 0.1 && data.OriAngles(i) >= -0.1){
            *i_best = i;
            break;
        }
    }
    if(*i_best == -1){
		int abs_min_idx = -1;
		int min_val = 99999;
		for(int i = 0; i < data.NumImages(); i++){
			if(abs(data.OriAngles(i)) < min_val){
				min_val = abs(data.OriAngles(i));
				abs_min_idx = i;
			}
		}
		*i_best = abs_min_idx;
    }

    int max_matches = MAX(data.GetNumMatches(*i_best, *i_best+1), data.GetNumMatches(*i_best-1, *i_best));

    if(max_matches < MATCH_THRESHOLD){
        *i_best = -1;
        *j_best = -1;
        EX_PRINT("Error: no good camera pairs found!\n")
        return -1;
    }

    int thres_matches = MAX(0.5/*0.4*/*max_matches, MATCH_THRESHOLD);

    float max_diff_angle = 0;
    for(int i = 0; i < num_images; i++){
        if(data.m_image_data[i].m_ignore_in_bundle)
            continue;

        if(i == *i_best)
            continue;

        int num_matches = data.GetNumMatches(i, *i_best);

        if(num_matches <= thres_matches){
            continue;
        }
        if(abs(data.m_image_data[i].m_angle_in_mrc) > max_diff_angle){
            max_diff_angle = abs(data.m_image_data[i].m_angle_in_mrc);
            *j_best = i;
        }
    }

    return data.GetNumMatches(*i_best, *j_best);
}

BundleApp::BundleApp(){}

BundleApp::~BundleApp()
{
    delete [] result;
}

void BundleApp::InitMotParam(float angle, double* params) const
{
    if(driver.Cnp() != 6){				//6+5
        Initating::InitKCalib5(params, width, height);
        Initating::InitRV3(params+5, angle);
    }
    else {
        Initating::InitRV3(params, angle);
    }
    Initating::InitTV3(params+driver.Cnp()-3, angle);
}

void BundleApp::InitCameraParam(float angle, mx::camera_params* camera) const
{
    double mot[11];
    InitMotParam(angle, mot);
    CameraCopyFormMotParams(camera, mot);
}

int BundleApp::SetupInitialCameraPair(int i_best, int j_best, mx::camera_params* cameras, v3_t* points, std::vector< ImageKeyVector >& pt_views)
{
    InitCameraParam(data.m_image_data[i_best].m_angle_in_mrc, cameras+0);
    InitCameraParam(data.m_image_data[j_best].m_angle_in_mrc, cameras+1);

    int pt_count = 0;
    MatchIndex list_idx = MatchIndex(i_best, j_best);
    std::vector<KeypointMatch> &list = data.m_matches.GetMatchList(list_idx);

    unsigned int num_matches = list.size();

    for(unsigned int i = 0; i < num_matches; i++){
        int key_idx1 = list[i].m_idx1;
        int key_idx2 = list[i].m_idx2;
//         EX_PRINT("  Adding match %d ==> %d [%d]\n", key_idx1, key_idx2, pt_count);

        double x_proj = data.GetKey(i_best,key_idx1).m_x;
        double y_proj = data.GetKey(i_best,key_idx1).m_y;

        /* Back project the point to a constant depth */
        util::_point ctra_pt = bundle::Initating::ContraMapping(util::_point(x_proj, y_proj), width, height, data.m_image_data[i_best].m_angle_in_mrc);
        x_proj = ctra_pt.x;
        y_proj = ctra_pt.y;
        
        double x_pt =(x_proj-width/2)/Initating::f*Initating::z;
        double y_pt =(y_proj-height/2)/Initating::f*Initating::z;
        double z_pt = Initating::z;
        

        points[pt_count] = v3_new(x_pt, y_pt, z_pt);

        data.GetKey(i_best,key_idx1).m_extra = pt_count;
        data.GetKey(j_best,key_idx2).m_extra = pt_count;

        int track_idx = data.GetKey(i_best,key_idx1).m_track;
        data.m_track_data[track_idx].m_extra = pt_count;

        ImageKeyVector views;
        views.push_back(ImageKey(0, key_idx1));
        views.push_back(ImageKey(1, key_idx2));
        pt_views.push_back(views);

        pt_count++;
    }

    return pt_count;
}

/* Find the camera with the most matches to existing points */
int BundleApp::FindCameraWithMostMatches(int* added_order, int num_cameras, int* max_matches, const std::vector<bundle::ImageKeyVector>& pt_views)
{
    * max_matches = 0;

    int i_best = -1;

    int num_images = data.NumImages();
    for(int i = 0; i < num_images; i++){
        if(data.m_image_data[i].m_ignore_in_bundle){
            continue;
        }

        /* Check if we added this image already */
        bool added = false;
        for(int j = 0; j < num_cameras; j++){
            if(added_order[j] == i){
                added = true;
                break;
            }
        }

        if(added){
            continue;
        }

        int num_existing_matches = 0;

        /* Find the tracks seen by this image */
        const std::vector<int>& tracks = data.m_image_data[i].m_visible_points;
        int num_tracks = (int)tracks.size();

        for(int j = 0; j < num_tracks; j++){
            int tr = tracks[j];
            if(data.m_track_data[tr].m_extra < 0){
                continue;
            }

            /* This tracks corresponds to a point */
            int pt = data.m_track_data[tr].m_extra;
            if((int)pt_views[pt].size()== 0){
                continue;
            }

            num_existing_matches++;
        }

        if(num_existing_matches >* max_matches){
            i_best = i;
            * max_matches = num_existing_matches;
        }
    }

    return i_best;
}

/** @brief Find all cameras with at least N matches to existing points */
std::vector<int> BundleApp::FindCamerasWithNMatches(int n, int* added_order, int num_cameras, const std::vector<bundle::ImageKeyVector> &pt_views)
{
    std::vector<int> new_images;

    int num_images = data.NumImages();
    for(int i = 0; i < num_images; i++){
        if(data.m_image_data[i].m_ignore_in_bundle){
            continue;
        }

        /* Check if we added this image already */
        bool added = false;
        for(int j = 0; j < num_cameras; j++){
            if(added_order[j] == i){
                added = true;
                break;
            }
        }

        if(added){
            continue;
        }

        int num_existing_matches = 0;

        /* Find the tracks seen by this image */
        const std::vector<int>& tracks = data.m_image_data[i].m_visible_points;
        int num_tracks = (int)tracks.size();

        for(int j = 0; j < num_tracks; j++){
            int tr = tracks[j];
            if(data.m_track_data[tr].m_extra < 0){
                continue;
            }

            int pt = data.m_track_data[tr].m_extra;
            if((int)pt_views[pt].size()== 0){
                continue;
            }

            num_existing_matches++;
        }

        if(num_existing_matches >= n){
            new_images.push_back(i);
        }
    }

    return new_images;
}

void BundleApp::BundleInitializeImage(int* added_order, mx::camera_params* camparams, int image_idx, int added_order_idx,
                                      v3_t* points, int num_points, std::vector<ImageKeyVector>& pt_views, bool* success_out)
{
    if(success_out != NULL){
        *success_out = true;
    }

    added_order[added_order_idx] = image_idx;
    InitCameraParam(data.m_image_data[image_idx].m_angle_in_mrc, &camparams[added_order_idx]);

    double P[12];

    /** *** Connect the new camera to any existing points* *** */
    EX_PRINT("Initialize Camera %d[%d]...\n", added_order_idx, image_idx);

    /* Find the tracks seen by this image */
    std::vector<int> &tracks = data.m_image_data[image_idx].m_visible_points;
    int num_tracks =(int)tracks.size();

    int num_pts_solve = 0;

    for(int i = 0; i < num_tracks; i++){
        int tr = tracks[i];
        if(data.m_track_data[tr].m_extra < 0){
            continue;
        }

        /* This tracks corresponds to a point */
        int pt = data.m_track_data[tr].m_extra;
        if((int)pt_views[pt].size()== 0){
            continue;
        }

        num_pts_solve++;
    }

    if(num_pts_solve < m_min_max_matches){
        EX_PRINT("Couldn't initialize Image [%d]\n", image_idx);

        if(success_out != NULL){
            *success_out = false;
        }
        return;
    }

    for(int i = 0; i < num_tracks; i++){
        int tr = tracks[i];
        if(data.m_track_data[tr].m_extra < 0){
            continue;
        }

        /* This tracks corresponds to a point */
        int pt = data.m_track_data[tr].m_extra;
        if((int)pt_views[pt].size()== 0){
            continue;
        }

        int key = data.m_image_data[image_idx].m_visible_keys[i];

        data.GetKey(image_idx, key).m_extra = pt;
        pt_views[pt].push_back(ImageKey(added_order_idx, key));
    }

    /** *** Finally, start the bundle adjustment* *** */
    driver.Run(added_order, camparams, 0, added_order_idx+1, added_order_idx, points, num_points, num_points, pt_views, data, false);

    return;
}

inline v2_t NormalizePoint(v2_t p)
{
    double r = sqrt(Vx(p)*Vx(p) + Vy(p)*Vy(p));

    return v2_scale(1/r, p);
}

/** @brief Triangulate a subtrack */
v3_t BundleApp::TriangulateNViews(const ImageKeyVector& views, int* added_order, mx::camera_params* cameras, double& error)
{
    int num_views = (int) views.size();

    v2_t* pv = new v2_t[num_views];
    double* Ps = new double[12*num_views];
    
    int num_valid = 0;

    for(int i = 0; i < num_views; i++){
        mx::camera_params* cam = NULL;

        int camera_idx = views[i].first;
        int image_idx = added_order[camera_idx];
        
        int key_idx = views[i].second;
        Keypoint& key = data.GetKey(image_idx, key_idx);

        pv[i] = v2_new(key.m_x, key.m_y);

        cam = cameras + camera_idx;
        mx::MxPFromCameraParams(*cam, Ps+12*i);
        num_valid++;
    }

    v3_t pt = bundle::triangulate_n(num_valid, pv, Ps, &error);

    delete [] pv;
    delete [] Ps;

    return pt;
}

/* Add new points to the bundle adjustment */
int BundleApp::BundleAdjustAddAllNewPoints(int num_points, int num_cameras, int* added_order, mx::camera_params* camparams, 
                 v3_t* points, std::vector<ImageKeyVector>& pt_views, double max_reprojection_error, int min_views)
{
    std::vector<int> track_idxs;
    std::vector<ImageKeyVector> new_tracks;

    //__gnu_cxx::hash_map<int,bool> tracks_seen;
    int num_tracks_total =(int)data.m_track_data.size();
    int* tracks_seen = new int[num_tracks_total];
    for(int i = 0; i < num_tracks_total; i++){
        tracks_seen[i] = -1;
    }

    /* Gather up the projections of all the new tracks */
    for(int i = 0; i < num_cameras; i++){
        int image_idx1 = added_order[i];

        int num_keys = data.GetNumKeys(image_idx1);

        for(int j = 0; j < num_keys; j++){
            Keypoint& key = data.GetKey(image_idx1, j);

            if(key.m_track == -1){
                continue; /* Key belongs to no track */
            }

            if(key.m_extra != -1){
                continue; /* Key is outlier or has already been added */
            }

            int track_idx = key.m_track;

            /* Check if this track is already associated with a point */
            if(data.m_track_data[track_idx].m_extra != -1){
                continue;
            }

            /* Check if we've seen this track */
            int seen = tracks_seen[track_idx];

            if(seen == -1){
                /* We haven't yet seen this track, create a new track */
                tracks_seen[track_idx] = (int)new_tracks.size();

                ImageKeyVector track;
                track.push_back(ImageKey(i, j));
                new_tracks.push_back(track);
                track_idxs.push_back(track_idx);
            }
            else {
                new_tracks[seen].push_back(ImageKey(i, j));
            }
        }
    }

    delete [] tracks_seen;

    /* Now for each(sub)track, triangulate to see if the track is consistent */
    int pt_count = num_points;

    int num_added = 0;

    int num_tracks =(int)new_tracks.size();
    for(int i = 0; i < num_tracks; i++){
        int num_views =(int)new_tracks[i].size();

        if(num_views < min_views) continue; 			// Not enough views

        double error;
        v3_t pt;

        int camera_idx = new_tracks[i][0].first;
        int image_idx = added_order[camera_idx];
        int key_idx = new_tracks[i][0].second;
        Keypoint& key = data.GetKey(image_idx, key_idx);
        double P_inv[12], C[4];
        MxCFromCameraParams(camparams[camera_idx], C);
        MxPinvFromCameraParams(camparams[camera_idx], P_inv);
// 	Initating::InverseProjection(util::_point(key.m_x, key.m_y), P_inv, C, pt.p);
        pt = TriangulateNViews(new_tracks[i], added_order, camparams, error);

        points[pt_count] = pt;
        pt_views.push_back(new_tracks[i]);

        /* Set the point index on the keys */
        for(int j = 0; j < num_views; j++){
            int camera_idx = new_tracks[i][j].first;
            int image_idx = added_order[camera_idx];
            int key_idx = new_tracks[i][j].second;
            data.GetKey(image_idx, key_idx).m_extra = pt_count;
        }

        data.m_track_data[track_idxs[i]].m_extra = pt_count;

        pt_count++;
        num_added++;
    }

    EX_PRINT("------Added %d new points\n", num_added);

    return pt_count;
}

void BundleApp::Process()
{
    int* added_order;
    std::vector<ImageKeyVector> pt_views;
    mx::camera_params* camparams;

    added_order = new int[data.NumImages()];
//     added_order_inv = new int[data.NumImages()];
    camparams = new mx::camera_params[data.NumImages()];		//as the order of added_order

    int max_pts =(int)data.m_track_data.size();
    v3_t* points = new v3_t[max_pts];

    int i_best, j_best;
    PickInitialPair(&i_best, &j_best);

    added_order[0] = i_best;
    added_order[1] = j_best;

    int pt_count = SetupInitialCameraPair(i_best, j_best, camparams, points, pt_views);

    driver.Run(added_order, camparams, 0, 2, 0, points, pt_count, 0, pt_views, data);

    int curr_num_cameras = 2;

    int round = 0;
    while(curr_num_cameras < data.NumImages()){
        int max_matches;
        int max_cam = FindCameraWithMostMatches(added_order, curr_num_cameras, &max_matches, pt_views);

        EX_PRINT("# max_matches = %d\n", max_matches);

        if(max_matches < m_min_max_matches){
            break;
        }

        /* Find all images with 90% of the matches of the maximum */
        std::vector<int> image_set;

        int match_num = MAX(int(0.5*max_matches), m_min_max_matches);
        image_set = FindCamerasWithNMatches(match_num, added_order, curr_num_cameras, pt_views);

        int num_added_images =(int)image_set.size();

        EX_PRINT("# Registering %d images", num_added_images);
        for(int i = 0; i < num_added_images; i++){
            EX_PRINT("[%d] ", image_set[i]);
        }
        EX_PRINT("\n")

        /* Now, throw the new cameras into the mix */
        int image_count = 0;
        for(int i = 0; i < num_added_images; i++){
            int next_idx = image_set[i];

            // Set up the new camera
            bool success = false;
            BundleInitializeImage(added_order, camparams, /*53*/next_idx, curr_num_cameras+image_count, points, pt_count, pt_views, &success);

            if(success){
                image_count++;
            }
            else {
                EX_PRINT("Warning: Couldn't initialize image %d\n", next_idx);
                data.m_image_data[next_idx].m_ignore_in_bundle = true;
            }
        }

        EX_PRINT("         ......\n");

        curr_num_cameras += image_count;

        pt_count = BundleAdjustAddAllNewPoints(pt_count, curr_num_cameras, added_order, camparams, points, pt_views, 16, 3);

        EX_PRINT("Bundle: Number of points = %d\n", pt_count);

        /* Run sfm again to update parameters */
        driver.Run(added_order, camparams, 0, curr_num_cameras, 0, points, pt_count, 0, pt_views, data);
        
        round++;
    }

    result = new mx::camera_params[data.NumImages()];

    for(int i = 0; i < data.NumImages(); i++){
        result[i].valid = false;
    }

    for(int i = 0; i < curr_num_cameras; i++){
        int img = added_order[i];
        if(!data.m_image_data[img].m_ignore_in_bundle){
            result[img] = camparams[i];
            result[img].valid = true;
        }
        else {
            result[img].valid = false;
        }
    }

    EX_NOTE(PrintRepErrorBin(camparams, curr_num_cameras, added_order, points);)

    /* Points */
    for(int i = 0; i < pt_count; i++){
        /* Check if the point is visible in any view */
        if((int)pt_views[i].size()== 0)
            continue;/* Invisible */

        PointData pdata;
        pdata.m_pos[0] = Vx(points[i]);
        pdata.m_pos[1] = Vy(points[i]);
        pdata.m_pos[2] = Vz(points[i]);

#ifdef FULL_INFO
        for(int j = 0; j <(int)pt_views[i].size(); j++){
            int v = pt_views[i][j].first;
            int vnew = added_order[v];
            pdata.m_views.push_back(ImageKey(vnew, pt_views[i][j].second));
        }
#endif
        m_point_data.push_back(pdata);
    }

    delete [] added_order;
    delete [] camparams;
    delete [] points;
//     std::ofstream out("camera_C.txt");
//     for(int i = 0; i < data.NumImages(); i++){
// 	PrintMxCofCamera(camparams[i], out);
//     }
//     out.close();
}

void BundleApp::PrintCameras(const char* filename) const
{
    std::ofstream out(filename);
    for(int i = 0; i < data.NumImages(); i++){
        if(result[i].valid){
            out<<"Camera ["<<i<<"]\n R:";
            result[i].R.Print(out);
            out<<" t:";
            result[i].T.Print(out);
        }
        else {
            out<<"Camera ["<<i<<"] Invalid"<<std::endl;
        }
    }

    out.close();
}

void BundleApp::PrintPoints(const char* filename) const
{
    std::ofstream out(filename);
    for(int i = 0; i < m_point_data.size(); i++){
        out<<m_point_data[i].m_pos[0]<<" "<<m_point_data[i].m_pos[1]<<" "<<m_point_data[i].m_pos[2]<<std::endl;
    }

    out.close();
}

void BundleApp::PrintTrackBin(const char* filename) const
{
    int bin[data.NumImages()+1];
    memset(bin, 0, sizeof(int)*(data.NumImages()+1));
    for(int i = 0; i < data.m_track_data.size(); i++){
        bin[data.m_track_data[i].m_views.size()]++;
    }
    std::ofstream out(filename);
    for(int i = data.NumImages(); i >= 2; i--){
        out<<i<<"\t"<<bin[i]<<std::endl;
    }
    out.close();
}

void BundleApp::PrintRepErrorBin(mx::camera_params* camparams, int ncams, const int added_order[],
                                 const v3_t* points, const char* foldername) const
{
    if(access(foldername,0) == -1){		//create file folder
        mkdir(foldername,0777);
    }
    std::ostringstream ooo;
    ooo <<foldername<<"/global.hist";
    std::ofstream out(ooo.str().c_str());

    int hist[81];
    float hist_gap = 0.05;
    int total = 0;
    memset(hist, 0, sizeof(int)*81);

    for(int i = 0; i < ncams; i++){
        double P[12];
        MxPFromCameraParams(camparams[i], P);

        int num_keys = data.GetNumKeys(added_order[i]);

        int num_pts_proj = 0;
        for(int j = 0; j < num_keys; j++){
            if(data.GetKey(added_order[i], j).m_extra >= 0){
                num_pts_proj++;
            }
        }

        double *dists = new double[num_pts_proj];
        int pt_count = 0;

        std::vector<Keypoint>::const_iterator iter;

        for(iter = data.m_image_data[added_order[i]].m_keys.begin(); iter != data.m_image_data[added_order[i]].m_keys.end(); iter++){
            const Keypoint &key = *iter;

            if(key.m_extra >= 0){
                int pt_idx = key.m_extra;
                double X[4], pr[3];
                memcpy(X, points[pt_idx].p, sizeof(double)*3);
                X[3] = 1;
                matrix_product(3, 4, 4, 1, P, X, pr);
                pr[0] /= pr[2];
                pr[1] /= pr[2];
                double dx = pr[0]-key.m_x;
                double dy = pr[1]-key.m_y;

                double dist = sqrt(dx * dx + dy * dy);
                int glo_num = round(dist/hist_gap) <= 80 ? round(dist/hist_gap) : 80;
                hist[glo_num]++;
                total++;
// 		global_dist.push_back(dist);

                dists[pt_count] = dist;

                pt_count++;
            }
        }

#define NUM_ERROR_BINS 10
        std::sort(dists, dists+num_pts_proj);

        double pr_min = dists[0];
        double pr_max = dists[num_pts_proj-1];
        double pr_step =(pr_max - pr_min)/NUM_ERROR_BINS;

        /* Break histogram into 10 bins */
        std::ostringstream oss;
        oss <<foldername<<"/cam"<<added_order[i]<<".hist";
        std::ofstream o(oss.str().c_str());

        int idx_count = 0;
        for(int i = 0; i < NUM_ERROR_BINS; i++){
            double max = pr_min +(i+1)* pr_step;
            int start = idx_count;

            while(idx_count < num_pts_proj && dists[idx_count] <= max){
                idx_count++;
            }

            int bin_size = idx_count - start;
            o<<"   ["<<max-pr_step<<"~"<<max<<"]: \t"<<bin_size<<"\t"<<bin_size/(double)num_pts_proj<<std::endl;
        }
        o.close();

        delete [] dists;
    }

    for(int i = 0; i < 81; i++){
        out<<"   ["<<i*hist_gap<<"]: \t"<<hist[i]<<"\t"<<hist[i]/(double)total<<std::endl;
    }
    out.close();
}

void BundleApp::DumpOutCameras(std::vector< mx::camera_params >* cameras) const
{
    for(int i = 0; i < data.NumImages(); i++){
        cameras->push_back(result[i]);
    }
}

void BundleApp::DumpOutPoints(std::vector< v3_t >* points) const
{
    for(int i = 0; i < m_point_data.size(); i++){
        v3_t tmp;
        memcpy(tmp.p, m_point_data[i].m_pos, sizeof(double)*3);

        points->push_back(tmp);
    }
}

void BundleApp::BundleMain(util::TrackSpace& tspace_, int width_, int height_, std::vector<mx::camera_params>* cameras, std::vector<v3_t>* points)
{
    BundleApp app;
    app.InitGeometryData(tspace_);
    app.InitBoundary(width_, height_);
    app.Process();
    app.DumpOutCameras(cameras);
    app.DumpOutPoints(points);
    app.PrintCameras();
    app.PrintPoints();
}

void BundleApp::Test()
{

}