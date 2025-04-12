import java.util.*;
import java.io.*;

public class Main {
    private static int N, M;
    private static int[][] map;

    private static boolean[][][] medusaArea;
    private static int[][] warriorMap;
    private static int[][] nxtWarriorMap;

    private static int[] medusa = new int[2];
    private static int[] park = new int[2];

    private static Queue<int[]> warriors = new ArrayDeque<>();

    private static int[] dr = {-1, 1, 0, 0};
    private static int[] dc = {0, 0, -1, 1};
    public static void main(String[] args) throws IOException {
        input();

        StringBuilder sb = new StringBuilder();
        // 메두사의 위치가 park 면 종료
        
        boolean[][] visit = new boolean[N][N];

        while(medusa[0] != park[0] || medusa[1] != park[1]){
            visit[medusa[0]][medusa[1]] = true;
            // 메두사 이동
            // 도로따라 이동
            if(!step1(sb, visit)) break;

            // 메두사 시선
            // 상하좌우 중, 가장 적을 많이 없앨 수 있는 방향을 쳐다봄, 90도 시야각, 나보다 앞에 선 전사가 있었으면 패스
            int[] step2Info = step2(); // killCount, 석화 방향
            int medusaKillCount = step2Info[0];
            int medusaAreaDir = step2Info[1];

            // 전사 이동 
            int warriorMove = step3_1(medusaAreaDir);
            warriorMove += step3_2(medusaAreaDir);
            
            // 전사의 공격
            // 메두사와 같은 칸 도달하면 소멸
            int attackCount = step4();
            
            sb.append(warriorMove).append(" ").append(medusaKillCount).append(" ").append(attackCount);
            sb.append("\n");
        }

        System.out.println(sb.toString());
    }
    
    private static boolean step1(StringBuilder sb, boolean[][] visit){
        PriorityQueue<int[]> pq = new PriorityQueue<>((o1, o2)->{
            if(o1[0] == o2[0]) return o1[1] - o2[1];
            return o1[0] - o2[0];
        });

        for(int i = 0; i < 4; i++){
            int nr = medusa[0] + dr[i];
            int nc = medusa[1] + dc[i];
            if(nr < 0 || nr >= N || nc < 0 || nc >= N || visit[nr][nc] || map[nr][nc] == 1) continue;
            int dist = -1;

            PriorityQueue<int[]> bfsPQ = new PriorityQueue<>((o1, o2) -> o1[0] - o2[0]);
            bfsPQ.add(new int[]{0, nr, nc});

            boolean[][] bfsVisit = new boolean[N][N];
            bfsVisit[nr][nc] = true;
            while(!bfsPQ.isEmpty()){
                int[] p = bfsPQ.poll();
                if(p[1] == park[0] && p[2] == park[1]){
                    dist = p[0];
                    break;
                }

                for(int j = 0; j < 4; j++){
                    int nnr = p[1] + dr[j];
                    int nnc = p[2] + dc[j];
                    if(nnr < 0 || nnr >= N || nnc < 0 || nnc >= N || map[nnr][nnc] == 1 || bfsVisit[nnr][nnc]) continue;
                    bfsVisit[nnr][nnc] = true;
                    bfsPQ.add(new int[]{p[0] + 1, nnr, nnc});
                }
            }

            if(dist == -1) continue;
            pq.add(new int[]{dist, i, nr, nc});
        }

        if(pq.isEmpty()){
            sb.delete(0, sb.length());
            sb.append("-1");
            return false;
        }

        int[] item = pq.poll();
        medusa[0] = item[2];
        medusa[1] = item[3];
        
        if(medusa[0] == park[0] && medusa[1] == park[1]){
            sb.append(0);
            return false;
        }

        if(warriorMap[medusa[0]][medusa[1]] > 0){
            int len = warriors.size();
            for(int i = 0; i < len; i++){
                int[] pos = warriors.poll();
                if(pos[0] == medusa[0] && pos[1] == medusa[1]) continue;
                warriors.add(pos);
            }
            warriorMap[medusa[0]][medusa[1]] = 0;
        }

        return true;
    }

    private static int[] step2(){
        PriorityQueue<int[]> pq = new PriorityQueue<>((o1, o2)->{
            if(o1[0] == o2[0]) return o1[1] - o2[1];
            return o2[0] - o1[0];
        });

        medusaArea = new boolean[4][N][N];
        for(int i = 0; i < 4; i++){
            boolean[] rock = new boolean[N];
            int killCount = 0;

            for(int j = 1; j < N; j++){
                int r = medusa[0] + (j * dr[i]);
                int c = medusa[1] + (j * dc[i]);
                if(r < 0 || r >= N || c < 0 || c >= N) break;

                medusaArea[i][r][c] = true;
                if(warriorMap[r][c] > 0){
                    if(i <= 1) rock[c] = true;
                    else rock[r] = true;

                    killCount += warriorMap[r][c];
                    break;
                }
            }
            
            int[] clone = medusa.clone();
            while(0 <= clone[0] && clone[0] < N && 0 <= clone[1] && clone[1] < N){
                boolean leftCHK = false, rightCHK = false;

                for(int j = 1; j <= N && (!leftCHK || !rightCHK); j++){
                    // 상 or 하, r좌표는 기존값 * j / c좌표는 -j, +j
                    // 좌 or 우, r좌표는 -j, +j / c좌표는 기존값 * j
                    int r = i <= 1 ? clone[0] + (dr[i] * j) : clone[0];
                    int c = i <= 1 ? clone[1] : clone[1] + (dc[i] * j);

                    if(i <= 1 && r < 0 || r >= N) break;
                    if(i > 1 && c < 0 || c >= N) break;

                    if(i <= 1){
                        if(!leftCHK && 0 <= c - j){
                            if(rock[c - j]) leftCHK = true;
                            else{
                                medusaArea[i][r][c - j] = true;
                                if(warriorMap[r][c - j] > 0){
                                    rock[c - j] = true;
                                    leftCHK = true;
                                    killCount += warriorMap[r][c - j];
                                }
                            }
                        }

                        if(!rightCHK && c + j < N){
                            if(rock[c + j]) rightCHK = true;
                            else{
                                medusaArea[i][r][c + j] = true;
                                if(warriorMap[r][c + j] > 0){
                                    rock[c + j] = true;
                                    rightCHK = true;
                                    killCount += warriorMap[r][c + j];
                                }
                            }
                        }

                    }else{
                        if(!leftCHK && 0 <= r - j){
                            if(rock[r - j]) leftCHK = true;
                            else{
                                medusaArea[i][r - j][c] = true;
                                if(warriorMap[r - j][c] > 0){
                                    rock[r - j] = true;
                                    leftCHK = true;
                                    killCount += warriorMap[r - j][c];
                                }
                            }
                        }

                        if(!rightCHK && r + j < N){
                            if(rock[r + j]) rightCHK = true;
                            else{
                                medusaArea[i][r + j][c] = true;
                                if(warriorMap[r + j][c] > 0){
                                    rock[r + j] = true;
                                    rightCHK = true;
                                    killCount += warriorMap[r + j][c];
                                }
                            }
                        }
                    }
                }
                clone[0] += dr[i];
                clone[1] += dc[i];
            }
            
            pq.add(new int[]{killCount, i});
        }

        int[] result = pq.poll();

        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                if(medusaArea[result[1]][i][j] && warriorMap[i][j] > 0){
                    nxtWarriorMap[i][j] = warriorMap[i][j];
                    warriorMap[i][j] = 0;
                }
            }
        }
        return result;
    }

    private static int step3_1(int medusaDir){
        int sum = 0;

        int len = warriors.size();
        for(int i = 0; i < len; i++){
            int[] pos = warriors.poll();

            if(warriorMap[pos[0]][pos[1]] == 0) continue;
            if(medusa[0] == pos[0] && medusa[1] == pos[1]){
                warriors.add(pos);
                continue;
            }

            int originDist = getDistFromTo(pos, medusa);
            warriorMap[pos[0]][pos[1]]--;

            PriorityQueue<int[]> pq = new PriorityQueue<>((o1, o2) -> {
                if(o1[0] == o2[0]) return o1[1] - o2[1];
                return o1[0] - o2[0];
            });

            for(int j = 0; j < 4; j++){
                int nr = pos[0] + dr[j];
                int nc = pos[1] + dc[j];
                if(nr < 0 || nr >= N || nc < 0 || nc >= N || medusaArea[medusaDir][nr][nc]) continue;
                int dist = getDistFromTo(new int[]{nr, nc}, medusa); 
                if(originDist < dist) continue;

                pq.add(new int[]{dist, j, nr, nc});
            }
            
            if(pq.isEmpty()){
                warriorMap[pos[0]][pos[1]]++;
                warriors.add(pos);
                continue;
            }

            int[] result = pq.poll();
            sum++;
            warriors.add(new int[]{result[2], result[3]});

            warriorMap[result[2]][result[3]]++;
        }

        return sum;
    }

    private static int step3_2(int medusaDir){
        int sum = 0;

        int len = warriors.size();
        for(int i = 0; i < len; i++){
            int[] pos = warriors.poll();
            if(medusa[0] == pos[0] && medusa[1] == pos[1]){
                warriors.add(pos);
                continue;
            }

            int originDist = getDistFromTo(pos, medusa);

            PriorityQueue<int[]> pq = new PriorityQueue<>((o1, o2) -> {
                if(o1[0] == o2[0]) return o1[1] - o2[1];
                return o1[0] - o2[0];
            });

            warriorMap[pos[0]][pos[1]]--;

            for(int j = 2; j < 6; j++){
                int nr = pos[0] + dr[j % 4];
                int nc = pos[1] + dc[j % 4];
                if(nr < 0 || nr >= N || nc < 0 || nc >= N || medusaArea[medusaDir][nr][nc]) continue;
                int dist = getDistFromTo(new int[]{nr, nc}, medusa);
                if(originDist < dist) continue;

                pq.add(new int[]{dist, j, nr, nc});
            }

            if(pq.isEmpty()){
                warriorMap[pos[0]][pos[1]]++;
                warriors.add(pos);
                continue;
            }

            int[] result = pq.poll();
            sum++;
            warriors.add(new int[]{result[2], result[3]});

            warriorMap[result[2]][result[3]]++;
        }
        
        return sum;
    }

    private static int step4(){
        int result = 0;

        int len = warriors.size();
        for(int i = 0; i < len; i++){
            int[] pos = warriors.poll();
            if(pos[0] == medusa[0] && pos[1] == medusa[1]){
                result++;
                warriorMap[pos[0]][pos[1]]--;
                continue;
            }

            warriors.add(pos);
        }
        
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                for(int k = 0; k < nxtWarriorMap[i][j]; k++){
                    warriors.add(new int[]{i, j});
                    warriorMap[i][j]++;
                }

                nxtWarriorMap[i][j] = 0;
            }
        }

        return result;
    }

    private static void input() throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringTokenizer stk = new StringTokenizer(br.readLine());
        N = Integer.parseInt(stk.nextToken());
        M = Integer.parseInt(stk.nextToken());

        map = new int[N][N];

        // 메두사 집과 공원 좌표
        stk = new StringTokenizer(br.readLine());
        medusa[0] = Integer.parseInt(stk.nextToken());
        medusa[1] = Integer.parseInt(stk.nextToken());
        
        park[0] = Integer.parseInt(stk.nextToken());
        park[1] = Integer.parseInt(stk.nextToken());

        // 전사 입력
        warriorMap = new int[N][N];
        nxtWarriorMap = new int[N][N];

        stk = new StringTokenizer(br.readLine());
        for(int i = 0; i < M; i++){
            int r = Integer.parseInt(stk.nextToken());
            int c = Integer.parseInt(stk.nextToken());

            warriorMap[r][c]++;
            warriors.add(new int[]{r,c});
        }

        // 도로 정보
        for(int i = 0; i < N; i++){
            stk = new StringTokenizer(br.readLine());
            for(int j = 0; j < N; j++){
                map[i][j] = Integer.parseInt(stk.nextToken());
            }
        }
    }
 
    private static int getDistFromTo(int[] from, int[] to){
        return Math.abs(from[0] - to[0]) + Math.abs(from[1] - to[1]);
    }
}